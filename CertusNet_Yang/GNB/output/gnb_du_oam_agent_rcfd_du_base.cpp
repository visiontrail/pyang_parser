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
    auto ns_yt = xconfd_yang_tree_get_node(yt, "ns");
    read_ns(ns_yt);
    auto drx_yt = xconfd_yang_tree_get_node(yt, "drx");
    read_drx(drx_yt);
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

void oam_agent_rcfd_du_base::read_ns(XCONFD_YANGTREE_T* yt)
{
    read_grp_ns(yt, ns_);
}

void oam_agent_rcfd_du_base::read_drx(XCONFD_YANGTREE_T* yt)
{
    if (!yt) return;
    drx_ = std::make_shared<Drx>();

    read_grp_drx(yt, *drx_);
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

void oam_agent_rcfd_du_base::read_grp_ns_cell(XCONFD_YANGTREE_T* yt, NsCell& ns_cell)
{
    xconfd_get(ns_cell.cell_id, uint8, "cell-id", yt);
    xconfd_get_empty_value(ns_cell.en_res_shared, "en-res-shared", yt);
    xconfd_get(ns_cell.res_id, uint8, "res-id", yt);
    xconfd_get(ns_cell.dl_num_ue_per_tti, uint8, "dl-num-ue-per-tti", yt);
    xconfd_get(ns_cell.ul_num_ue_per_tti, uint8, "ul-num-ue-per-tti", yt);
    xconfd_get(ns_cell.dl_prb, uint16, "dl-prb", yt);
    xconfd_get(ns_cell.ul_prb, uint16, "ul-prb", yt);
    xconfd_get(ns_cell.dl_sla_trgt_rate, uint64, "dl-sla-trgt-rate", yt);
    xconfd_get(ns_cell.ul_sla_trgt_rate, uint64, "ul-sla-trgt-rate", yt);
}

void oam_agent_rcfd_du_base::read_grp_ip_addr(XCONFD_YANGTREE_T* yt, IpAddr& ip_addr)
{
    xconfd_get(ip_addr.ip_version, enum, "ip-version", yt);
    xconfd_get_optional(ip_addr.ipv4_addr, Ipv4Address, ipv4, "ipv4-addr", yt);
    xconfd_get_optional(ip_addr.ipv6_addr, Ipv6Address, ipv6-address, "ipv6-addr", yt);
}

void oam_agent_rcfd_du_base::read_grp_ns(XCONFD_YANGTREE_T* yt, Ns& ns)
{
    xconfd_get(ns.agent_type, enum, "agent-type", yt);
    auto local_sm_yt = xconfd_yang_tree_get_node(yt, "local-sm");
    read_grp_ns__local_sm(local_sm_yt, ns.local_sm);
    auto remote_sm_yt = xconfd_yang_tree_get_node(yt, "remote-sm");
    read_grp_ns__remote_sm(remote_sm_yt, ns.remote_sm);
    auto nsis_yt = xconfd_yang_tree_get_node(yt, "nsis");
    read_grp_ns__nsis(nsis_yt, ns.nsis);
}

void oam_agent_rcfd_du_base::read_grp_ns__local_sm(XCONFD_YANGTREE_T* yt, std::shared_ptr<IpAddr>& local_sm)
{
    xconfd_get(local_sm->port, uint32, "port", yt);
    read_grp_ip_addr(yt, local_sm.ip_addr);
}

void oam_agent_rcfd_du_base::read_grp_ns__remote_sm(XCONFD_YANGTREE_T* yt, std::shared_ptr<IpAddr>& remote_sm)
{
    xconfd_get(remote_sm->port, uint32, "port", yt);
    read_grp_ip_addr(yt, remote_sm.ip_addr);
}

void oam_agent_rcfd_du_base::read_grp_ns__nsis(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<Nsi>>& nsis)
{
    XCONFD_YANG_TREE_LIST_FOREACH(yt, nsi_yt)
    {
        auto nsi = std::make_shared<Nsi>();
        read_grp_nsi(nsi_yt, *nsi);
        nsis.push_back(nsi);
    }
}

void oam_agent_rcfd_du_base::read_grp_nsi(XCONFD_YANGTREE_T* yt, Nsi& nsi)
{
    xconfd_get(nsi.nsi_id, uint8, "nsi-id", yt);
    xconfd_yang_tree_get_leaf_list(nsi.cores, uint16, "cores", yt);
    auto cells_yt = xconfd_yang_tree_get_node(yt, "cells");
    read_grp_nsi__cells(cells_yt, nsi.cells);
}

void oam_agent_rcfd_du_base::read_grp_nsi__cells(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<NsCell>>& cells)
{
    XCONFD_YANG_TREE_LIST_FOREACH(yt, ns_cell_yt)
    {
        auto ns_cell = std::make_shared<NsCell>();
        read_grp_ns_cell(ns_cell_yt, *ns_cell);
        cells.push_back(ns_cell);
    }
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


} // namespace rcfd 
} // namespace gnb_du