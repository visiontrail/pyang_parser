/*
 * filename: gnb_du_oam_agent_rcfd_du_ns.cpp 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#include "gnb_du_oam_agent_rcfd_du_ns.h" 

namespace gnb_du 
{
namespace rcfd
{

oam_agent_rcfd_du_ns::oam_agent_rcfd_du_ns(XCONFD_YANGTREE_T* yt)
{
}

void oam_agent_rcfd_du_ns::read_grp_ns_cell(XCONFD_YANGTREE_T* yt, NsCell& ns_cell)
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

void oam_agent_rcfd_du_ns::read_grp_ns(XCONFD_YANGTREE_T* yt, Ns& ns)
{
    xconfd_get(ns.agent_type, enum, "agent-type", yt);
    auto local_sm_yt = xconfd_yang_tree_get_node(yt, "local-sm");
    read_grp_ns__local_sm(local_sm_yt, ns.local_sm);
    auto remote_sm_yt = xconfd_yang_tree_get_node(yt, "remote-sm");
    read_grp_ns__remote_sm(remote_sm_yt, ns.remote_sm);
    auto nsis_yt = xconfd_yang_tree_get_node(yt, "nsis");
    read_grp_ns__nsis(nsis_yt, ns.nsis);
}

void oam_agent_rcfd_du_ns::read_grp_ns__local_sm(XCONFD_YANGTREE_T* yt, std::shared_ptr<IpAddr>& local_sm)
{
    xconfd_get(local_sm->port, uint32, "port", yt);
    read_grp_ip_addr(yt, local_sm.ip_addr);
}

void oam_agent_rcfd_du_ns::read_grp_ns__remote_sm(XCONFD_YANGTREE_T* yt, std::shared_ptr<IpAddr>& remote_sm)
{
    xconfd_get(remote_sm->port, uint32, "port", yt);
    read_grp_ip_addr(yt, remote_sm.ip_addr);
}

void oam_agent_rcfd_du_ns::read_grp_ns__nsis(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<Nsi>>& nsis)
{
    XCONFD_YANG_TREE_LIST_FOREACH(yt, nsi_yt)
    {
        auto nsi = std::make_shared<Nsi>();
        read_grp_nsi(nsi_yt, *nsi);
        nsis.push_back(nsi);
    }
}

void oam_agent_rcfd_du_ns::read_grp_nsi(XCONFD_YANGTREE_T* yt, Nsi& nsi)
{
    xconfd_get(nsi.nsi_id, uint8, "nsi-id", yt);
    xconfd_yang_tree_get_leaf_list(nsi.cores, uint16, "cores", yt);
    auto cells_yt = xconfd_yang_tree_get_node(yt, "cells");
    read_grp_nsi__cells(cells_yt, nsi.cells);
}

void oam_agent_rcfd_du_ns::read_grp_nsi__cells(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<NsCell>>& cells)
{
    XCONFD_YANG_TREE_LIST_FOREACH(yt, ns_cell_yt)
    {
        auto ns_cell = std::make_shared<NsCell>();
        read_grp_ns_cell(ns_cell_yt, *ns_cell);
        cells.push_back(ns_cell);
    }
}


} // namespace rcfd 
} // namespace gnb_du