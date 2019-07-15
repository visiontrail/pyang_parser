/*
 * filename: gnb_du_oam_agent_rcfd_cell_l1.cpp 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#include "gnb_du_oam_agent_rcfd_cell_l1.h" 

namespace gnb_du 
{
namespace rcfd
{

oam_agent_rcfd_cell_l1::oam_agent_rcfd_cell_l1(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(dl_central_freq_, uint32, "dl-central-freq", yt);
    xconfd_get(ul_central_freq_, uint32, "ul-central-freq", yt);
    xconfd_get(ca_lvl_, uint8, "ca-lvl", yt);
    auto prach_yt = xconfd_yang_tree_get_node(yt, "prach");
    read_prach(prach_yt);
    auto mib_yt = xconfd_yang_tree_get_node(yt, "mib");
    read_mib(mib_yt);
    auto sib1_yt = xconfd_yang_tree_get_node(yt, "sib1");
    read_sib1(sib1_yt);
}

void oam_agent_rcfd_cell_l1::read_prach(XCONFD_YANGTREE_T* yt)
{
    if (!yt) return;
    prach_ = std::make_shared<Prach>();

    read_grp_prach(yt, *prach_);
}

void oam_agent_rcfd_cell_l1::read_mib(XCONFD_YANGTREE_T* yt)
{
    read_grp_mib(yt, mib_);
}

void oam_agent_rcfd_cell_l1::read_sib1(XCONFD_YANGTREE_T* yt)
{
    read_grp_sib1(yt, sib1_);
}

void oam_agent_rcfd_cell_l1::read_grp_cell_access_info(XCONFD_YANGTREE_T* yt, CellAccessInfo& cell_access_info)
{
    auto plmn_id_infos_yt = xconfd_yang_tree_get_node(yt, "plmn-id-infos");
    read_grp_cell_access_info__plmn_id_infos(plmn_id_infos_yt, cell_access_info.plmn_id_infos);
    xconfd_get_empty_value(cell_access_info.cell_rsrvd_for_other_use, "cell-rsrvd-for-other-use", yt);
}

void oam_agent_rcfd_cell_l1::read_grp_cell_access_info__plmn_id_infos(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<PlmnIdInfo>>& plmn_id_infos)
{
    XCONFD_YANG_TREE_LIST_FOREACH(yt, plmn_id_info_yt)
    {
        auto plmn_id_info = std::make_shared<PlmnIdInfo>();
        read_grp_plmn_id_info(plmn_id_info_yt, *plmn_id_info);
        plmn_id_infos.push_back(plmn_id_info);
    }
}

void oam_agent_rcfd_cell_l1::read_grp_plmn_id_info(XCONFD_YANGTREE_T* yt, PlmnIdInfo& plmn_id_info)
{
    auto plmn_ids_yt = xconfd_yang_tree_get_node(yt, "plmn-ids");
    read_grp_plmn_id_info__plmn_ids(plmn_ids_yt, plmn_id_info.plmn_ids);
    xconfd_get(plmn_id_info.nr_cell_id, uint64, "nr-cell-id", yt);
    xconfd_get_optional(plmn_id_info.tac, uint32_t, uint32, "tac", yt);
    xconfd_get_optional(plmn_id_info.ranac, uint8_t, uint8, "ranac", yt);
    xconfd_get_empty_value(plmn_id_info.cell_rsrvd_for_operator_use, "cell-rsrvd-for-operator-use", yt);
}

void oam_agent_rcfd_cell_l1::read_grp_plmn_id_info__plmn_ids(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<PlmnId>>& plmn_ids)
{
    XCONFD_YANG_TREE_LIST_FOREACH(yt, plmn_id_yt)
    {
        auto plmn_id = std::make_shared<PlmnId>();
        read_grp_plmn_id(plmn_id_yt, *plmn_id);
        plmn_ids.push_back(plmn_id);
    }
}

void oam_agent_rcfd_cell_l1::read_grp_sib1(XCONFD_YANGTREE_T* yt, Sib1& sib1)
{
    auto cell_sel_info_yt = xconfd_yang_tree_get_node(yt, "cell-sel-info");
    read_grp_sib1__cell_sel_info(cell_sel_info_yt, sib1.cell_sel_info);
    auto cell_access_info_yt = xconfd_yang_tree_get_node(yt, "cell-access-info");
    read_grp_sib1__cell_access_info(cell_access_info_yt, sib1.cell_access_info);
    auto ue_timers_yt = xconfd_yang_tree_get_node(yt, "ue-timers");
    read_grp_sib1__ue_timers(ue_timers_yt, sib1.ue_timers);
}

void oam_agent_rcfd_cell_l1::read_grp_sib1__cell_sel_info(XCONFD_YANGTREE_T* yt, CellSelInfo& cell_sel_info)
{
    read_grp_cell_sel_info(yt, cell_sel_info);
}

void oam_agent_rcfd_cell_l1::read_grp_sib1__cell_access_info(XCONFD_YANGTREE_T* yt, CellAccessInfo& cell_access_info)
{
    read_grp_cell_access_info(yt, cell_access_info);
}

void oam_agent_rcfd_cell_l1::read_grp_sib1__ue_timers(XCONFD_YANGTREE_T* yt, UeTimers& ue_timers)
{
    read_grp_ue_timers(yt, ue_timers);
}

void oam_agent_rcfd_cell_l1::read_grp_mib(XCONFD_YANGTREE_T* yt, Mib& mib)
{
    xconfd_get(mib.scs_cmn, enum, "scs-cmn", yt);
    xconfd_get(mib.ssb_sc_offset, uint8, "ssb-sc-offset", yt);
    auto pdcch_cfg_sib1_yt = xconfd_yang_tree_get_node(yt, "pdcch-cfg-sib1");
    read_grp_mib__pdcch_cfg_sib1(pdcch_cfg_sib1_yt, mib.pdcch_cfg_sib1);
    xconfd_get_empty_value(mib.cell_barred, "cell-barred", yt);
    xconfd_get_empty_value(mib.intra_freq_resel, "intra-freq-resel", yt);
}

void oam_agent_rcfd_cell_l1::read_grp_mib__pdcch_cfg_sib1(XCONFD_YANGTREE_T* yt, PdcchCfgSib1& pdcch_cfg_sib1)
{
    xconfd_get(pdcch_cfg_sib1.coreset_zero, uint8, "coreset-zero", yt);
    xconfd_get(pdcch_cfg_sib1.ss_zero, uint8, "ss-zero", yt);
}

void oam_agent_rcfd_cell_l1::read_grp_ue_timers(XCONFD_YANGTREE_T* yt, UeTimers& ue_timers)
{
    xconfd_get(ue_timers.t300, enum, "t300", yt);
    xconfd_get(ue_timers.t301, enum, "t301", yt);
    xconfd_get(ue_timers.t310, enum, "t310", yt);
    xconfd_get(ue_timers.n310, enum, "n310", yt);
    xconfd_get(ue_timers.t311, enum, "t311", yt);
    xconfd_get(ue_timers.n311, enum, "n311", yt);
    xconfd_get(ue_timers.t319, enum, "t319", yt);
}

void oam_agent_rcfd_cell_l1::read_grp_cell_sel_info(XCONFD_YANGTREE_T* yt, CellSelInfo& cell_sel_info)
{
    xconfd_get_optional(cell_sel_info.q_rx_lvl_min, QRxLvlMinT, int8, "q-rx-lvl-min", yt);
    xconfd_get_optional(cell_sel_info.q_rx_lvl_min_offset, uint8_t, uint8, "q-rx-lvl-min-offset", yt);
    xconfd_get_optional(cell_sel_info.q_rx_lvl_min_sul, QRxLvlMinT, int8, "q-rx-lvl-min-sul", yt);
    xconfd_get_optional(cell_sel_info.q_qual_min, QQualMinT, int8, "q-qual-min", yt);
    xconfd_get_optional(cell_sel_info.q_qual_min_offset, uint8_t, uint8, "q-qual-min-offset", yt);
}

void oam_agent_rcfd_cell_l1::read_grp_prach(XCONFD_YANGTREE_T* yt, Prach& prach)
{
    xconfd_get(prach.root_seq_idx, uint16, "root-seq-idx", yt);
    xconfd_get(prach.cfg_idx, uint8, "cfg-idx", yt);
    xconfd_get(prach.zero_correlation_zone_cfg, uint8, "zero-correlation-zone-cfg", yt);
    xconfd_get_optional(prach.restricted_set, uint8_t, uint8, "restricted-set", yt);
    xconfd_get(prach.freq_start, uint16, "freq-start", yt);
    xconfd_get(prach.freq_offset, uint8, "freq-offset", yt);
    xconfd_get(prach.scs, enum, "scs", yt);
    xconfd_get(prach.fdm, enum, "fdm", yt);
    xconfd_get(prach.ssb_per_rach_occ, enum, "ssb-per-rach-occ", yt);
}


} // namespace rcfd 
} // namespace gnb_du