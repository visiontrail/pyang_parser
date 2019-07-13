/*!********************************************************************************************************************
 * file        gnb_du_oam_agent_rcfd_cell.cpp
 *
 * history       
 *********************************************************************************************************************/

#include "xconfd_api.h"
#include "gnb_du_oam_agent_msg.h"
#include "gnb_du_oam_agent_msg_comm.h"
#include "gnb_du_oam_agent_rcfd_cell.h"

#define XPATH_DU_CELLS "/du-cells"
#define CELL_ADMIN_STATE   XPATH_DU_CELLS"/admin-state"
#define CELL_BASE          XPATH_DU_CELLS"/cell-base"
#define CELL_SCHD          XPATH_DU_CELLS"/cell-schd"
#define CELL_L1            XPATH_DU_CELLS"/cell-l1"
#define CELL_CHANNEL       XPATH_DU_CELLS"/cell-channel"

#define NR_CELL_ID_01      CELL_BASE"/nr-cgi/nr-cell-id"
#define NR_CELL_ID_02      CELL_L1"/sib1/cell-access-info/plmn-id-infos/nr-cell-id"

#define FREQ_DOMAIN_RES_01 CELL_SCHD"/fdd-cfg-cmn/dl-cfg-cmn/initial-bwp/pdcch-cfg-cmn/coreset/freq-domain-res"
#define FREQ_DOMAIN_RES_02 CELL_SCHD"/addtl-bwp-cfg/dl-bwps/pdcch-cfg-cmn/coreset/freq-domain-res"


namespace gnb_du
{
namespace rcfd
{

oam_agent_rcfd_cells* oam_agent_rcfd_cells::instance_ = nullptr;

oam_agent_rcfd_cell::oam_agent_rcfd_cell(XCONFD_YANGTREE_T* yt)
{
   xconfd_get(admin_state_, enum, "admin-state", yt);

   auto base_yt = xconfd_yang_tree_get_node(yt, "cell-base-cfg");
   base_ = std::make_shared<oam_agent_rcfd_cell_base>(base_yt);

   auto schd_yt = xconfd_yang_tree_get_node(yt, "cell-schd-cfg");
   schd_ = std::make_shared<oam_agent_rcfd_cell_schd>(schd_yt);

   auto l1_yt = xconfd_yang_tree_get_node(yt, "cell-l1-cfg");
   l1_ = std::make_shared<oam_agent_rcfd_cell_l1>(l1_yt);

   auto channel_yt = xconfd_yang_tree_get_node(yt, "cell-channel-cfg");
   channel_ = std::make_shared<oam_agent_rcfd_cell_channel>(channel_yt);
}


ret_t oam_agent_rcfd_cells::init()
{
   xconfd_reg_conf_list_m(cell_add_del_notify, XPATH_DU_CELLS);
   xconfd_reg_conf_list_m(cell_admin_state_modified_notify, XPATH_DU_CELLS, "admin-state");
   xconfd_reg_conf_full_m(cell_base_modified_notify, CELL_BASE);
   xconfd_reg_conf_full_m(cell_schd_modified_notify, CELL_SCHD);
   xconfd_reg_conf_full_m(cell_l1_modified_notify, CELL_L1);
   xconfd_reg_conf_full_m(cell_channel_modified_notify, CELL_CHANNEL);

   xconfd_reg_validate_m(nr_cell_id_check, "nr-cell-id-check", NR_CELL_ID_01, NR_CELL_ID_02);
   xconfd_reg_validate_m(freq_domain_res_check, "freq-domain-res-check", FREQ_DOMAIN_RES_01, FREQ_DOMAIN_RES_02);
   
   return ret_t::SUCCESS;
}

ret_t oam_agent_rcfd_cells::deinit()
{
   if (instance_)
   {
      delete instance_;
      instance_ = nullptr;
   }
   return ret_t::SUCCESS;
}

const rcfd_cell_ptr oam_agent_rcfd_cells::get_rcfd_cell(uint32_t cell_id)
{
   return rcfd_cells_.find(cell_id) == rcfd_cells_.end() ? nullptr : rcfd_cells_[cell_id];
}

void oam_agent_rcfd_cells::get_rcfd_unblocked_cells(std::vector<uint32_t>& cells)
{
   for (auto& cell : rcfd_cells_)
   {
      if (cell.second->get_admin_state() == BLOCKED)
      {
         continue;
      }

      cells.push_back(cell.first);
   }
}

ret_t oam_agent_rcfd_cells::set_cell_oper_state(uint32_t cell_id, OperStateE oper_state)
{
   std::string xpath = XPATH_DU_CELLS;
   xpath += "{" + std::to_string(cell_id) + "}" + "oper-state";

   if (xconfd_commit_value(xconfd_newvalue_enum(oper_state), xpath.c_str()) == XCONFD_OK)
   {
      return ret_t::SUCCESS;
   }
   else
   {
      return ret_t::FAILURE;
   }
}

int oam_agent_rcfd_cells::cell_add_del_notify(XCONFD_CTX_T *ctx, XCONFD_YANGTREE_T* yt, void* usrdata)
{
   auto cell_id = xconfd_get_uint32(xconfd_yang_tree_get_value_ex(yt, "cell-id"));
   auto& rcfd_cells = oam_agent_rcfd_cells::get_instance()->rcfd_cells_;

   auto op = xconfd_yang_tree_get_operation(yt);
   if (op == XCONFD_OP_CREATE)
   {
      rcfd_cells[cell_id] = std::make_shared<oam_agent_rcfd_cell>(yt);
      if (rcfd_cells[cell_id]->get_admin_state() == BLOCKED)
      {
         return 0;
      }

      auto unblock_req = new oam_gnb_cfd_unblock_cell_request();
      unblock_req->set_cell_id(cell_id);
      gnb_du_oam_agent_msg_comm::get_instance()->send(&cmGCb, unblock_req);
   }
   else if (op == XCONFD_OP_DELETED)
   {
      oam_agent_rcfd_cells::get_instance()->rcfd_cells_.erase(cell_id);
   }
   else
   {
      // unexpected
   }

   return 0;
}

int oam_agent_rcfd_cells::cell_admin_state_modified_notify(XCONFD_CTX_T *ctx, XCONFD_YANGTREE_T* yt, void* usrdata)
{
   auto rcfd_cell = oam_agent_rcfd_cells::get_instance()->get_cell(yt);
   if (!rcfd_cell)
   {
      return 0;
   }

   AdminStateE admin_state = rcfd_cell->get_admin_state();
   xconfd_get(admin_state, enum, "admin-state", yt);
   if (admin_state == rcfd_cell->get_admin_state())
   {
      return 0;
   }
   rcfd_cell->set_admin_state(admin_state);

   auto cell_id = xconfd_get_uint32(xconfd_yang_tree_get_value_ex(yt, "../cell-id"));
   if (admin_state == UNBLOCKED)
   {
      auto unblock_req = new oam_gnb_cfd_unblock_cell_request();
      unblock_req->set_cell_id(cell_id);
      gnb_du_oam_agent_msg_comm::get_instance()->send(&cmGCb, unblock_req);
   }
   else
   {
      auto block_req = new oam_gnb_cfd_block_cell_request();
      block_req->set_cell_id(cell_id);
      gnb_du_oam_agent_msg_comm::get_instance()->send(&cmGCb, block_req);
   }

   return 0;
}

int oam_agent_rcfd_cells::cell_base_modified_notify(XCONFD_CTX_T *ctx, XCONFD_YANGTREE_T* yt, void* usrdata)
{
   auto rcfd_cell = oam_agent_rcfd_cells::get_instance()->get_cell(yt);
   if (rcfd_cell)
   {
      rcfd_cell->reread_base_info(yt);
   }

   return 0;
}

int oam_agent_rcfd_cells::cell_schd_modified_notify(XCONFD_CTX_T *ctx, XCONFD_YANGTREE_T* yt, void* usrdata)
{
   auto rcfd_cell = oam_agent_rcfd_cells::get_instance()->get_cell(yt);
   if (rcfd_cell)
   {
      rcfd_cell->reread_schd_info(yt);
   }

   return 0;
}

int oam_agent_rcfd_cells::cell_l1_modified_notify(XCONFD_CTX_T *ctx, XCONFD_YANGTREE_T* yt, void* usrdata)
{
   auto rcfd_cell = oam_agent_rcfd_cells::get_instance()->get_cell(yt);
   if (rcfd_cell)
   {
      rcfd_cell->reread_l1_info(yt);
   }

   return 0;
}

int oam_agent_rcfd_cells::cell_channel_modified_notify(XCONFD_CTX_T *ctx, XCONFD_YANGTREE_T* yt, void* usrdata)
{
   auto rcfd_cell = oam_agent_rcfd_cells::get_instance()->get_cell(yt);
   if (rcfd_cell)
   {
      rcfd_cell->reread_channel_info(yt);
   }

   return 0;
}

int oam_agent_rcfd_cells::nr_cell_id_check(XCONFD_CTX_T* ctx, XCONFD_YANGTREE_T* yt, void* user)
{
   auto nr_cell_id = xconfd_get_uint64(xconfd_yang_tree_get_value_ex(yt, "nr-cell-id"));
   if ((nr_cell_id & 0x0F) != 0)
   {
      return xconfd_validate_reply_error(ctx, "BIT STRING(36)");
   }

   return xconfd_validate_reply_ok(ctx);
}

int oam_agent_rcfd_cells::freq_domain_res_check(XCONFD_CTX_T* ctx, XCONFD_YANGTREE_T* yt, void* user)
{
   auto freq_domain_res = xconfd_get_uint64(xconfd_yang_tree_get_value_ex(yt, "freq-domain-res"));
   if ((freq_domain_res & 0x07) != 0)
   {
      return xconfd_validate_reply_error(ctx, "BIT STRING(45)");
   }

   return xconfd_validate_reply_ok(ctx);
}

const rcfd_cell_ptr oam_agent_rcfd_cells::get_cell(XCONFD_YANGTREE_T* yt)
{
   auto cell_id = xconfd_get_uint32(xconfd_yang_tree_get_value_ex(yt, "../cell-id"));
   if (rcfd_cells_.find(cell_id) != rcfd_cells_.end())
   {
      return rcfd_cells_[cell_id];
   }

   return nullptr;
}

} // namespace rcfd
} // namespace gnb_du

