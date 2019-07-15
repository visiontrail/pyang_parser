/*!********************************************************************************************************************
 * file        gnb_du_oam_agent_rcfd_cell.cpp
 *
 * history       
 *********************************************************************************************************************/

#include "gnb_du_oam_agent_rcfd_cell_base.h"


namespace gnb_du
{
namespace rcfd
{

oam_agent_rcfd_cell_base::oam_agent_rcfd_cell_base(XCONFD_YANGTREE_T* yt)
{
   xconfd_get(nr_mode_type_, enum, "nr-mode-type", yt);
   
   auto nr_cgi_yt = xconfd_yang_tree_get_node(yt, "nr-cgi");
   read_nr_cgi(nr_cgi_yt);
   
   xconfd_get(nr_pci_, uint16, "nr-pci", yt);
   xconfd_get(tac_5gs_, uint32, "tac-5gs", yt);
   xconfd_get(cfgrd_eps_tac_, uint16, "cfgrd-eps-tac", yt);

   auto served_plmns_yt = xconfd_yang_tree_get_node(yt, "served-plmns");
   read_served_plmns(served_plmns_yt);   

   auto si_perd_yt = xconfd_yang_tree_get_node(yt, "si-perd");
   read_si_perd(si_perd_yt);
}

void oam_agent_rcfd_cell_base::read_nr_cgi(XCONFD_YANGTREE_T* yt)
{
   read_grp_plmn_id(yt, nr_cgi_.plmn_id);
   xconfd_get(nr_cgi_.nr_cell_id, uint64, "nr-cell-id", yt);
}

void oam_agent_rcfd_cell_base::read_served_plmns(XCONFD_YANGTREE_T* yt)
{
   XCONFD_YANG_TREE_LIST_FOREACH(yt, served_plmn_yt)
   {
      auto served_plmn = std::make_shared<ServedPlmn>();
      read_grp_plmn_id(served_plmn_yt, served_plmn->plmn_id);

      auto s_nssais_yt = xconfd_yang_tree_get_node(served_plmn_yt, "s-nssais");
      read_served_plmns__s_nssais(s_nssais_yt, served_plmn->s_nssais);

      served_plmns_.push_back(served_plmn);
   }
}

void oam_agent_rcfd_cell_base::read_si_perd(XCONFD_YANGTREE_T* yt)
{
   xconfd_get_optional_enum(si_perd_.sib2_perd, SiPerdE, "sib2-perd", yt);
   xconfd_get_optional_enum(si_perd_.sib3_perd, SiPerdE, "sib3-perd", yt);
}

void oam_agent_rcfd_cell_base::read_grp_s_nssai(XCONFD_YANGTREE_T* yt, SNssai& s_nssai)
{
   xconfd_get(s_nssai.sst, uint8, "sst", yt);
   xconfd_get(s_nssai.sd, uint32, "sd", yt);
}

void oam_agent_rcfd_cell_base::read_served_plmns__s_nssais(XCONFD_YANGTREE_T* yt, SNssaiPtrVector& s_nssais)
{
   XCONFD_YANG_TREE_LIST_FOREACH(yt, s_nssai_yt)
   {
      auto s_nssai = std::make_shared<SNssai>();
      read_grp_s_nssai(s_nssai_yt, *s_nssai);
      s_nssais.push_back(s_nssai);
   }
}





} // namespace rcfd
} // namespace gnb_du

