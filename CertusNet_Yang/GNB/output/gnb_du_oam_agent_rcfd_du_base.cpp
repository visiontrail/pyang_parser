/*
 * filename: gnb_du_oam_agent_rcfd_du_base.cpp 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#include "gnb_du_oam_agent_rcfd_du_base.h" 

namespace gnb_du 
{
namespace rcfd
{

oam_agent_rcfd_du_base::oam_agent_rcfd_du_base(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(gnb_du_id_, uint64, "gnb-du-id", yt);
    xconfd_get_optional(gnb_du_name_, std::string, string, "gnb-du-name", yt);
    xconfd_get(max_cell_supported_, uint8, "max-cell-supported", yt);
    auto ue_yt = xconfd_yang_tree_get_node(yt, "ue");
    read_ue(ue_yt);
    auto sctp_yt = xconfd_yang_tree_get_node(yt, "sctp");
    read_sctp(sctp_yt);
    auto egtpu_yt = xconfd_yang_tree_get_node(yt, "egtpu");
    read_egtpu(egtpu_yt);
    auto f1u_flow_ctrl_yt = xconfd_yang_tree_get_node(yt, "f1u-flow-ctrl");
    read_f1u_flow_ctrl(f1u_flow_ctrl_yt);
    auto drx_yt = xconfd_yang_tree_get_node(yt, "drx");
    read_drx(drx_yt);
    auto log_yt = xconfd_yang_tree_get_node(yt, "log");
    read_log(log_yt);
}

void oam_agent_rcfd_du_base::read_ue(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(ue_.max_ue_supported, uint16, "max-ue-supported", yt);
    xconfd_get(ue_.rnti_start, uint16, "rnti-start", yt);
    xconfd_get(ue_.max_num_rnti, uint16, "max-num-rnti", yt);
}

void oam_agent_rcfd_du_base::read_sctp(XCONFD_YANGTREE_T* yt)
{
    read_grp_sctp(yt, sctp_);
}

void oam_agent_rcfd_du_base::read_egtpu(XCONFD_YANGTREE_T* yt)
{
    read_grp_ip_addr(yt, egtpu_.ip_addr);
    xconfd_get(egtpu_.teid_min, uint32, "teid-min", yt);
    xconfd_get(egtpu_.teid_max, uint32, "teid-max", yt);
}

void oam_agent_rcfd_du_base::read_f1u_flow_ctrl(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(f1u_flow_ctrl_.max_rlc_sdu_q_size, uint16, "max-rlc-sdu-q-size", yt);
    xconfd_get(f1u_flow_ctrl_.rlc_sdu_q_lwr_thr, uint16, "rlc-sdu-q-lwr-thr", yt);
    xconfd_get(f1u_flow_ctrl_.rlc_sdu_q_upr_thr, uint16, "rlc-sdu-q-upr-thr", yt);
    xconfd_get(f1u_flow_ctrl_.nrup_flw_ctrl_tmr, uint16, "nrup-flw-ctrl-tmr", yt);
    xconfd_get_empty_value(f1u_flow_ctrl_.en_nrup_missing_rept, "en-nrup_missing_rept", yt);
    xconfd_get(f1u_flow_ctrl_.read_ingress_pkts_per_tti, uint32, "read-ingress-pkts-per-tti", yt);
}

void oam_agent_rcfd_du_base::read_drx(XCONFD_YANGTREE_T* yt)
{
    if (!yt) return;
    drx_ = std::make_shared<Drx>();

    read_grp_drx(yt, *drx_);
}

void oam_agent_rcfd_du_base::read_log(XCONFD_YANGTREE_T* yt)
{
    read_grp_log(yt, log_);
}

void oam_agent_rcfd_du_base::read_grp_drx(XCONFD_YANGTREE_T* yt, Drx& drx)
{
    xconfd_get(drx.inactivity_tmr, uint32, "inactivity-tmr", yt);
    xconfd_get(drx.retx_tmr_dl, uint32, "retx-tmr-dl", yt);
    xconfd_get(drx.retx_tmr_ul, uint32, "retx-tmr-ul", yt);
    xconfd_get(drx.long_cycle, uint32, "long-cycle", yt);
    xconfd_get(drx.short_cycle, uint32, "short-cycle", yt);
    xconfd_get(drx.short_cycle_tmr, uint32, "short-cycle-tmr", yt);
}

void oam_agent_rcfd_du_base::read_grp_sctp(XCONFD_YANGTREE_T* yt, Sctp& sctp)
{
    auto dst_yt = xconfd_yang_tree_get_node(yt, "dst");
    read_grp_sctp__dst(dst_yt, sctp.dst);
    auto src_yt = xconfd_yang_tree_get_node(yt, "src");
    read_grp_sctp__src(src_yt, sctp.src);
    auto cfg_params_yt = xconfd_yang_tree_get_node(yt, "cfg-params");
    read_grp_sctp__cfg_params(cfg_params_yt, sctp.cfg_params);
}

void oam_agent_rcfd_du_base::read_grp_sctp__dst(XCONFD_YANGTREE_T* yt, IpAddr& dst)
{
    xconfd_get(dst.port, uint32, "port", yt);
    read_grp_ip_addr(yt, dst.ip_addr);
}

void oam_agent_rcfd_du_base::read_grp_sctp__src(XCONFD_YANGTREE_T* yt, IpAddr& src)
{
    xconfd_get(src.port, uint32, "port", yt);
    read_grp_ip_addr(yt, src.ip_addr);
}

void oam_agent_rcfd_du_base::read_grp_sctp__cfg_params(XCONFD_YANGTREE_T* yt, CfgParams& cfg_params)
{
    xconfd_get(cfg_params.num_outbound_streams, uint16, "num-outbound-streams", yt);
    xconfd_get(cfg_params.max_inbound_streams, uint16, "max-inbound-streams", yt);
    xconfd_get(cfg_params.max_init_attempts, uint16, "max-init-attempts", yt);
    xconfd_get(cfg_params.hb_interval, uint16, "hb-interval", yt);
    xconfd_get(cfg_params.max_path_retx, uint16, "max-path-retx", yt);
}

void oam_agent_rcfd_du_base::read_grp_log(XCONFD_YANGTREE_T* yt, Log& log)
{
    xconfd_get(log.file_name, string, "file-name", yt);
    auto du_modules_yt = xconfd_yang_tree_get_node(yt, "du-modules");
    read_grp_log__du_modules(du_modules_yt, log.du_modules);
    auto ngp_modules_yt = xconfd_yang_tree_get_node(yt, "ngp-modules");
    read_grp_log__ngp_modules(ngp_modules_yt, log.ngp_modules);
}

void oam_agent_rcfd_du_base::read_grp_log__du_modules(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<DuModules>>& du_modules)
{
    xconfd_get(du_modules.module_id, enum, "module-id", yt);
    xconfd_get(du_modules.log_lvl, enum, "log-lvl", yt);
}

void oam_agent_rcfd_du_base::read_grp_log__ngp_modules(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<NgpModules>>& ngp_modules)
{
    xconfd_get(ngp_modules.module_id, enum, "module-id", yt);
    xconfd_get(ngp_modules.log_lvl, enum, "log-lvl", yt);
}


} // namespace rcfd 
} // namespace gnb_du