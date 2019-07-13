/*
 * filename: gnb_du_oam_agent_rcfd_cell_channel.cpp 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#include "gnb_du_oam_agent_rcfd_cell_channel.h" 

namespace gnb_du 
{
namespace rcfd
{

oam_agent_rcfd_cell_channel::oam_agent_rcfd_cell_channel(XCONFD_YANGTREE_T* yt)
{
    auto pcch_yt = xconfd_yang_tree_get_node(yt, "pcch");
    read_pcch(pcch_yt);

    auto bcch_yt = xconfd_yang_tree_get_node(yt, "bcch");
    read_bcch(bcch_yt);

    auto pusch_scell_yt = xconfd_yang_tree_get_node(yt, "pusch-scell");
    read_pusch_scell(pusch_scell_yt);

    auto pdsch_scell_yt = xconfd_yang_tree_get_node(yt, "pdsch-scell");
    read_pdsch_scell(pdsch_scell_yt);
}

void oam_agent_rcfd_cell_channel::read_pcch(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(pcch_.default_paging_cycle, enum, "default-paging-cycle", yt);
    xconfd_get(pcch_.paging_frame_offset_type, enum, "paging-frame-offset-type", yt);
    xconfd_get(pcch_.paging_frame_offset, uint8, "paging-frame-offset", yt);
    xconfd_get(pcch_.ns, enum, "ns", yt);
    xconfd_get_empty_value(pcch_.f_pdcch_mon_occ_of_po_present, "f-pdcch-mon-occ-of-po-present", yt);
}

void oam_agent_rcfd_cell_channel::read_bcch(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(bcch_.modi_perd_coeff, enum, "modi-perd-coeff", yt);
    xconfd_get(bcch_.si_win_len, enum, "si-win-len", yt);
}

void oam_agent_rcfd_cell_channel::read_pusch_scell(XCONFD_YANGTREE_T* yt)
{
    xconfd_get_optional_enum(pusch_scell_.max_cbgs_per_tb, MaxCbgsPerTbE, "max-cbgs-per-tb", yt);
    xconfd_get_empty_value(pusch_scell_.limited_buf_rm, "limited-buf-rm", yt);
    xconfd_get_optional_enum(pusch_scell_.x_overhead, XOverheadE, "x-overhead", yt);
}

void oam_agent_rcfd_cell_channel::read_pdsch_scell(XCONFD_YANGTREE_T* yt)
{
    xconfd_get_optional_enum(pdsch_scell_.max_cbgs_per_tb, MaxCbgsPerTbE, "max-cbgs-per-tb", yt);
    xconfd_get_empty_value(pdsch_scell_.cbg_flush_ind, "cbg-flush-ind", yt);
    xconfd_get_optional_enum(pdsch_scell_.x_overhead, XOverheadE, "x-overhead", yt);
    xconfd_get_optional_enum(pdsch_scell_.harq_processes_num, PdschHarqProcessesNumE, "harq-processes-num", yt);
    xconfd_get_optional(pdsch_scell_.pucch_cell_idx, uint8_t, uint8, "pucch-cell-idx", yt);
}

} // namespace rcfd 
} // namespace gnb_du