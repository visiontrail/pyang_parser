/*!********************************************************************************************************************
 * file        gnb_du_oam_agent_rcfd_schd.cpp
 *
 * history       
 *********************************************************************************************************************/

#include "gnb_du_oam_agent_rcfd_cell_schd.h"

#define XPATH_DU_CELLS__CELL_SCHD_CFG "/du-cells/cell-schd-cfg"
#define FREQ_DOMAIN_RES_01 XPATH_DU_CELLS__CELL_SCHD_CFG"/fdd-cfg-cmn/dl-cfg-cmn/initial-bwp/pdcch-cfg-cmn/coreset/freq-domain-res"
#define FREQ_DOMAIN_RES_02 XPATH_DU_CELLS__CELL_SCHD_CFG"/addtl-bwp-cfg/dl-bwps/pdcch-cfg-cmn/coreset/freq-domain-res"


namespace gnb_du
{
namespace rcfd_cell
{

oam_agent_rcfd_cell_schd::oam_agent_rcfd_cell_schd(XCONFD_YANGTREE_T* yt)
{
   xconfd_get(max_ue_per_ul_tti_, uint32, "max-ue-per-ul-tti", yt);
   xconfd_get(max_ue_per_ul_tti_, uint32, "max-ue-per-dl-tti", yt);
   xconfd_get(max_dl_harq_tx_, uint8, "max-dl-harq-tx", yt);
   xconfd_get(max_msg4_harq_tx_, uint32, "max-msg4-harq-tx", yt);
   xconfd_get(duration_, uint16, "duration", yt);
   xconfd_get(cmn_lcs_num_, uint8, "cmn-lcs-num", yt);
   xconfd_get_empty_value(cp_ul_extended_, "cp-ul-extended", yt);
   xconfd_get(scs_, enum, "scs", yt);
   xconfd_get(dmrs_type_a_pos_, enum, "dmrs-type-a-pos", yt);
   xconfd_get(slot_format_, enum, "slot-format", yt);
   xconfd_get(harq_ack_cb_type_, enum, "harq-ack-cb-type", yt);
   xconfd_get(bwp_inactive_timer_, enum, "bwp-inactive-timer", yt);
   xconfd_get_empty_value(scell_cmn_cfg_in_sib1_, "scell-cmn-cfg-in-sib1", yt);

   auto preamble_id_range_yt = xconfd_yang_tree_get_node(yt, "preamble-id-range");
   read_preamble_id_range(preamble_id_range_yt);

   auto fdd_cfg_cmn_yt = xconfd_yang_tree_get_node(yt, "fdd-cfg-cmn");
   read_fdd_cfg_cmn(fdd_cfg_cmn_yt);

   auto tdd_cfg_cmn_yt = xconfd_yang_tree_get_node(yt, "tdd-cfg-cmn");
   read_tdd_cfg_cmn(tdd_cfg_cmn_yt);

   auto sul_cfg_yt = xconfd_yang_tree_get_node(yt, "sul-cfg");
   read_sul_cfg(sul_cfg_yt);

   auto ss_pbch_cfg_yt = xconfd_yang_tree_get_node(yt, "ss-pbch-cfg");
   read_ss_pbch_cfg(ss_pbch_cfg_yt);

   auto addtl_bwp_cfg_yt = xconfd_yang_tree_get_node(yt, "addtl-bwp-cfg");
   read_addtl_bwp_cfg(addtl_bwp_cfg_yt);

   auto ue_bsr_cfg_yt = xconfd_yang_tree_get_node(yt, "ue-bsr-cfg");
   read_ue_bsr_cfg(ue_bsr_cfg_yt);
   
   /* Register configuration change */
   xconfd_reg_validate_m(freq_domain_res_check, "freq-domain-res-check", FREQ_DOMAIN_RES_01, FREQ_DOMAIN_RES_02);
}

void oam_agent_rcfd_cell_schd::read_preamble_id_range(XCONFD_YANGTREE_T* yt)
{
   xconfd_get(preamble_id_range_.start, uint16, "start", yt);
   xconfd_get(preamble_id_range_.size, uint16, "size", yt);
}

void oam_agent_rcfd_cell_schd::read_fdd_cfg_cmn(XCONFD_YANGTREE_T* yt)
{
   if (!yt) return;
   fdd_cfg_cmn_ = std::make_shared<FddCfgCmn>();

   auto ul_cfg_cmn_yt = xconfd_yang_tree_get_node(yt, "ul-cfg-cmn");
   read_fdd_cfg_cmn__ul_cfg_cmn(ul_cfg_cmn_yt, fdd_cfg_cmn_->ul_cfg_cmn);

   auto dl_cfg_cmn_yt = xconfd_yang_tree_get_node(yt, "dl-cfg-cmn");
   read_fdd_cfg_cmn__dl_cfg_cmn(dl_cfg_cmn_yt, fdd_cfg_cmn_->dl_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_fdd_cfg_cmn__ul_cfg_cmn(XCONFD_YANGTREE_T* yt, UlCfgCmn& ul_cfg_cmn)
{
   read_grp_ul_cfg_cmn(yt, ul_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_fdd_cfg_cmn__dl_cfg_cmn(XCONFD_YANGTREE_T* yt, DlCfgCmn& dl_cfg_cmn)
{
   read_grp_dl_cfg_cmn(yt, dl_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_cfg_cmn(XCONFD_YANGTREE_T* yt, UlCfgCmn& ul_cfg_cmn)
{
   xconfd_get_empty_value(ul_cfg_cmn.present_in_sib1, "present-in-sib1", yt);

   auto freq_info_yt = xconfd_yang_tree_get_node(yt, "freq-info");
   read_grp_ul_cfg_cmn__freq_info(freq_info_yt, ul_cfg_cmn.freq_info);

   auto initial_bwp_yt = xconfd_yang_tree_get_node(yt, "initial-bwp");
   read_grp_ul_cfg_cmn__initial_bwp(initial_bwp_yt, ul_cfg_cmn.initial_bwp);

   xconfd_get(ul_cfg_cmn.time_alignment_timer, enum, "time-alignment-timer", yt);

   auto ue_harq_cfg_yt = xconfd_yang_tree_get_node(yt, "ue-harq-cfg");
   read_grp_ul_cfg_cmn__ue_harq_cfg(ue_harq_cfg_yt, ul_cfg_cmn.ue_harq_cfg);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_cfg_cmn__freq_info(XCONFD_YANGTREE_T* yt, UlFreqInfo& freq_info)
{
   read_grp_ul_freq_info(yt, freq_info);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_freq_info(XCONFD_YANGTREE_T* yt, UlFreqInfo& ul_freq_info)
{
   auto freq_info_cmn_yt = xconfd_yang_tree_get_node(yt, "freq-info-cmn");
   read_grp_ul_freq_info__freq_info_cmn(freq_info_cmn_yt, ul_freq_info.freq_info_cmn);

   xconfd_get(ul_freq_info.addtl_spectrum_emi, uint8, "addtl-spectrum-emi", yt);
   xconfd_get(ul_freq_info.p_max, int8, "p-max", yt);
   xconfd_get_empty_value(ul_freq_info.freq_shift_7p5khz, "freq-shift-7p5khz", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_freq_info__freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn)
{
   read_grp_freq_info_cmn(yt, freq_info_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn)
{
   xconfd_get(freq_info_cmn.abs_freq_point_a, uint32, "abs-freq-point-a", yt);
   xconfd_get(freq_info_cmn.abs_arfcn_point_a, uint32, "abs-arfcn-point-a", yt);
   xconfd_get(freq_info_cmn.nr_arfcn, uint32, "nr-arfcn", yt);
   xconfd_get(freq_info_cmn.channel_bw, enum, "channel-bw", yt);
   xconfd_yang_tree_get_leaf_list(freq_info_cmn.nr_freq_bands, uint16, "nr-freq-bands", yt);

   auto scs_spec_carriers_yt = xconfd_yang_tree_get_node(yt, "scs-spec-carriers");
   read_grp_freq_info_cmn__scs_spec_carriers(scs_spec_carriers_yt, freq_info_cmn.scs_spec_carriers);
}

void oam_agent_rcfd_cell_schd::read_grp_freq_info_cmn__scs_spec_carriers(XCONFD_YANGTREE_T* yt, ScsSpecCarrierPtrVector& scs_spec_carriers)
{
   XCONFD_YANG_TREE_LIST_FOREACH(yt, scs_spec_carrier_yt)
   {
      read_grp_scs_spec_carrier(scs_spec_carrier_yt, scs_spec_carriers);
   }
}

void oam_agent_rcfd_cell_schd::read_grp_scs_spec_carrier(XCONFD_YANGTREE_T* yt, ScsSpecCarrierPtrVector& scs_spec_carriers)
{
   auto scs_spec_carrier = std::make_shared<ScsSpecCarrier>();

   xconfd_get(scs_spec_carrier->offset_to_carrier, uint16, "offset-to-carrier", yt);
   xconfd_get(scs_spec_carrier->scs, enum, "scs", yt);

   scs_spec_carriers.push_back(scs_spec_carrier);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_cfg_cmn__initial_bwp(XCONFD_YANGTREE_T* yt, UlBwpCmn& initial_bwp)
{
   read_grp_ul_bwp_cmn(yt, initial_bwp);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_bwp_cmn(XCONFD_YANGTREE_T* yt, UlBwpCmn& ul_bwp_cmn)
{
   auto bwp_gen_yt = xconfd_yang_tree_get_node(yt, "bwp-gen");
   read_grp_ul_bwp_cmn__bwp_gen(bwp_gen_yt, ul_bwp_cmn.bwp_gen);

   auto rach_cfg_cmn_yt = xconfd_yang_tree_get_node(yt, "rach-cfg-cmn");
   read_grp_ul_bwp_cmn__rach_cfg_cmn(rach_cfg_cmn_yt, ul_bwp_cmn.rach_cfg_cmn);

   auto pusch_cfg_cmn_yt = xconfd_yang_tree_get_node(yt, "pusch-cfg-cmn");
   read_grp_ul_bwp_cmn__pusch_cfg_cmn(pusch_cfg_cmn_yt, ul_bwp_cmn.pusch_cfg_cmn);

   auto pucch_cfg_cmn_yt = xconfd_yang_tree_get_node(yt, "pucch-cfg-cmn");
   read_grp_ul_bwp_cmn__pucch_cfg_cmn(pucch_cfg_cmn_yt, ul_bwp_cmn.pucch_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_bwp_cmn__bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen)
{
   read_grp_bwp_gen(yt, bwp_gen);
}

void oam_agent_rcfd_cell_schd::read_grp_bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen)
{
   xconfd_get(bwp_gen.start_rb, uint16, "start-rb", yt);
   xconfd_get(bwp_gen.rb_num, uint16, "rb-num", yt);
   xconfd_get(bwp_gen.scs, enum, "scs", yt);
   xconfd_get_empty_value(bwp_gen.cp_extended, "cp-extended", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_bwp_cmn__rach_cfg_cmn(XCONFD_YANGTREE_T* yt, RachCfgCmn& rach_cfg_cmn)
{
   read_grp_rach_cfg_cmn(yt, rach_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_rach_cfg_cmn(XCONFD_YANGTREE_T* yt, RachCfgCmn& rach_cfg_cmn)
{
   auto rach_gen_cfg_yt = xconfd_yang_tree_get_node(yt, "rach-gen-cfg");
   read_grp_rach_cfg_cmn__rach_gen_cfg(rach_gen_cfg_yt, rach_cfg_cmn.rach_gen_cfg);

   xconfd_get(rach_cfg_cmn.total_num_ra_preamble, uint8, "total-num-ra-preamble", yt);
   xconfd_get(rach_cfg_cmn.ssb_per_rach_occ, enum, "ssb-per-rach-occ", yt);
   xconfd_get(rach_cfg_cmn.cb_preamble_per_ssb, uint8, "cb-preamble-per-ssb", yt);

   auto rach_grpb_cfg_yt = xconfd_yang_tree_get_node(yt, "rach-grpb-cfg");
   read_grp_rach_cfg_cmn__rach_grpb_cfg(rach_grpb_cfg_yt, rach_cfg_cmn.rach_grpb_cfg);

   xconfd_get(rach_cfg_cmn.ra_cont_res_tmr, enum, "ra-cont-res-tmr", yt);
   xconfd_get(rach_cfg_cmn.prach_root_seq_idx_type, enum, "prach-root-seq-idx-type", yt);
   xconfd_get(rach_cfg_cmn.prach_root_seq_idx, uint16, "prach-root-seq-idx", yt);
   xconfd_get(rach_cfg_cmn.msg1_scs, enum, "msg1-scs", yt);
   xconfd_get(rach_cfg_cmn.restr_set_cfg, enum, "restr-set-cfg", yt);
   xconfd_get_empty_value(rach_cfg_cmn.msg3_tran_precoding, "msg3-tran-precoding", yt);
   xconfd_get(rach_cfg_cmn.max_msg3_trans, uint8, "max-msg3-trans", yt);
   xconfd_get(rach_cfg_cmn.rsrp_thres_ssb, uint8, "rsrp-thres-ssb", yt);
   xconfd_get(rach_cfg_cmn.rsrp_thres_ssb_sul, uint8, "rsrp-thres-ssb-sul", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_rach_cfg_cmn__rach_gen_cfg(XCONFD_YANGTREE_T* yt, RachGenCfg& rach_gen_cfg)
{
   read_grp_rach_gen_cfg(yt, rach_gen_cfg);
}

void oam_agent_rcfd_cell_schd::read_grp_rach_gen_cfg(XCONFD_YANGTREE_T* yt, RachGenCfg& rach_gen_cfg)
{
   xconfd_get(rach_gen_cfg.prach_cfg_idx, uint8, "prach-cfg-idx", yt);
   xconfd_get(rach_gen_cfg.msg1_fdm, enum, "msg1-fdm", yt);
   xconfd_get(rach_gen_cfg.msg1_freq_start, uint16, "msg1-freq-start", yt);
   xconfd_get(rach_gen_cfg.zero_corre_zone_cfg, uint8, "zero-corre-zone-cfg", yt);
   xconfd_get(rach_gen_cfg.preamble_rx_tpower, int16, "preamble-rx-tpower", yt);
   xconfd_get(rach_gen_cfg.preamble_trans_max, enum, "preamble-trans-max", yt);
   xconfd_get(rach_gen_cfg.power_ramp_step, enum, "power-ramp-step", yt);
   xconfd_get(rach_gen_cfg.ra_rsp_win, enum, "ra-rsp-win", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_rach_cfg_cmn__rach_grpb_cfg(XCONFD_YANGTREE_T* yt, RachGrpbCfg& rach_grpb_cfg)
{
   read_grp_rach_grpb_cfg(yt, rach_grpb_cfg);
}

void oam_agent_rcfd_cell_schd::read_grp_rach_grpb_cfg(XCONFD_YANGTREE_T* yt, RachGrpbCfg& rach_grpb_cfg)
{
   xconfd_get(rach_grpb_cfg.ra_msg3_size_grp_a, enum, "ra-msg3-size-grp-a", yt);
   xconfd_get(rach_grpb_cfg.num_ra_preamble_grp_a, uint8, "num-ra-preamble-grp-a", yt);
   xconfd_get(rach_grpb_cfg.msg_power_offset_grp_b, enum, "msg-power-offset-grp-b", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_bwp_cmn__pusch_cfg_cmn(XCONFD_YANGTREE_T* yt, PuschCfgCmn& pusch_cfg_cmn)
{
   read_grp_pusch_cfg_cmn(yt, pusch_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_pusch_cfg_cmn(XCONFD_YANGTREE_T* yt, PuschCfgCmn& pusch_cfg_cmn)
{
   xconfd_get_empty_value(pusch_cfg_cmn.grp_hopping_trans_precoding, "grp-hopping-trans-precoding", yt);
   xconfd_get(pusch_cfg_cmn.msg3_delta_preamble, int8, "msg3-delta-preamble", yt);
   xconfd_get(pusch_cfg_cmn.p0_nominal_with_grant, int16, "p0-nominal-with-grant", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_bwp_cmn__pucch_cfg_cmn(XCONFD_YANGTREE_T* yt, PucchCfgCmn& pucch_cfg_cmn)
{
   read_grp_pucch_cfg_cmn(yt, pucch_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_pucch_cfg_cmn(XCONFD_YANGTREE_T* yt, PucchCfgCmn& pucch_cfg_cmn)
{
   xconfd_get(pucch_cfg_cmn.res_cmn, uint8, "res-cmn", yt);
   xconfd_get(pucch_cfg_cmn.grp_hopping, enum, "grp-hopping", yt);
   xconfd_get(pucch_cfg_cmn.hopping_id, uint16, "hopping-id", yt);
   xconfd_get(pucch_cfg_cmn.p0_nominal, int16, "p0-nominal", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_ul_cfg_cmn__ue_harq_cfg(XCONFD_YANGTREE_T* yt, UeHarqCfg& ue_harq_cfg)
{
   xconfd_get(ue_harq_cfg.max_harq, uint8, "max-harq", yt);
   xconfd_get(ue_harq_cfg.delta_harq_offset, uint8, "delta-harq-offset", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_dl_cfg_cmn(XCONFD_YANGTREE_T* yt, DlCfgCmn& dl_cfg_cmn)
{
   auto freq_info_yt = xconfd_yang_tree_get_node(yt, "freq-info");
   read_grp_dl_cfg_cmn__freq_info(freq_info_yt, dl_cfg_cmn.freq_info);

   auto initial_bwp_yt = xconfd_yang_tree_get_node(yt, "initial-bwp");
   read_grp_dl_cfg_cmn__initial_bwp(initial_bwp_yt, dl_cfg_cmn.initial_bwp);
}

void oam_agent_rcfd_cell_schd::read_grp_dl_cfg_cmn__freq_info(XCONFD_YANGTREE_T* yt, DlFreqInfo& freq_info)
{
   read_grp_dl_freq_info(yt, freq_info);
}

void oam_agent_rcfd_cell_schd::read_grp_dl_freq_info(XCONFD_YANGTREE_T* yt, DlFreqInfo& dl_freq_info)
{
   auto freq_info_cmn_yt = xconfd_yang_tree_get_node(yt, "freq-info-cmn");
   read_grp_dl_freq_info__freq_info_cmn(freq_info_cmn_yt, dl_freq_info.freq_info_cmn);

   xconfd_get(dl_freq_info.abs_freq_ssb, uint32, "abs-freq-ssb", yt);
   xconfd_get(dl_freq_info.abs_arfcn_ssb, uint32, "abs-arfcn-ssb", yt);
   xconfd_get(dl_freq_info.offset_to_point_a, uint16, "offset-to-point-a", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_dl_freq_info__freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn)
{
   read_grp_freq_info_cmn(yt, freq_info_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_dl_cfg_cmn__initial_bwp(XCONFD_YANGTREE_T* yt, DlBwpCmn& initial_bwp)
{
   read_grp_dl_bwp_cmn(yt, initial_bwp);
}

void oam_agent_rcfd_cell_schd::read_grp_dl_bwp_cmn(XCONFD_YANGTREE_T* yt, DlBwpCmn& dl_bwp_cmn)
{
   auto bwp_gen_yt = xconfd_yang_tree_get_node(yt, "bwp-gen");
   read_grp_dl_bwp_cmn__bwp_gen(bwp_gen_yt, dl_bwp_cmn.bwp_gen);

   auto pdcch_cfg_cmn_yt = xconfd_yang_tree_get_node(yt, "pdcch-cfg-cmn");
   read_grp_dl_bwp_cmn__pdcch_cfg_cmn(pdcch_cfg_cmn_yt, dl_bwp_cmn.pdcch_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_dl_bwp_cmn__bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen)
{
   read_grp_bwp_gen(yt, bwp_gen);
}

void oam_agent_rcfd_cell_schd::read_grp_dl_bwp_cmn__pdcch_cfg_cmn(XCONFD_YANGTREE_T* yt, PdcchCfgCmn& pdcch_cfg_cmn)
{
   read_grp_pdcch_cfg_cmn(yt, pdcch_cfg_cmn);
}

void oam_agent_rcfd_cell_schd::read_grp_pdcch_cfg_cmn(XCONFD_YANGTREE_T* yt, PdcchCfgCmn& pdcch_cfg_cmn)
{
   auto coreset_yt = xconfd_yang_tree_get_node(yt, "coreset");
   read_grp_pdcch_cfg_cmn__coreset(coreset_yt, pdcch_cfg_cmn.coreset);

   auto search_spaces_yt = xconfd_yang_tree_get_node(yt, "search-spaces");
   read_grp_pdcch_cfg_cmn__search_spaces(search_spaces_yt, pdcch_cfg_cmn.search_spaces);

   xconfd_get(pdcch_cfg_cmn.ss_sib1, uint8, "ss-sib1", yt);
   xconfd_get(pdcch_cfg_cmn.ss_other_si, uint8, "ss-other-si", yt);
   xconfd_get(pdcch_cfg_cmn.ss_paging, uint8, "ss-paging", yt);
   xconfd_get(pdcch_cfg_cmn.ss_ra, uint8, "ss-ra", yt);
   xconfd_get(pdcch_cfg_cmn.coreset_zero, uint8, "coreset-zero", yt);
   xconfd_get(pdcch_cfg_cmn.ss_zero, uint8, "ss-zero", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_pdcch_cfg_cmn__coreset(XCONFD_YANGTREE_T* yt, Coreset& coreset)
{
   read_grp_coreset(yt, coreset);
}

void oam_agent_rcfd_cell_schd::read_grp_coreset(XCONFD_YANGTREE_T* yt, Coreset& coreset)
{
   xconfd_get(coreset.id, uint8, "id", yt);
   xconfd_get(coreset.freq_domain_res, uint64, "freq-domain-res", yt);
   xconfd_get(coreset.duration, uint8, "duration", yt);

   auto cce_reg_mapping_interleaved_yt = xconfd_yang_tree_get_node(yt, "cce-reg-mapping-interleaved");
   read_grp_coreset__cce_reg_mapping_interleaved(cce_reg_mapping_interleaved_yt, coreset.interleaved);

   xconfd_get(coreset.precoder_granularity, enum, "precoder-granularity", yt);
   xconfd_get(coreset.dmrs_scrambl_id, uint16, "dmrs-scramble-id", yt);

   auto tci_state_info_yt = xconfd_yang_tree_get_node(yt, "tci-state-info");
   read_grp_coreset__tci_state_info(tci_state_info_yt, coreset.tci_state_info);
}

void oam_agent_rcfd_cell_schd::read_grp_coreset__cce_reg_mapping_interleaved(
   XCONFD_YANGTREE_T* yt, CceRegMappingInterleaved& cce_reg_mapping_interleaved)
{
   xconfd_get(cce_reg_mapping_interleaved.reg_bndl_size, enum, "reg-bndl-size", yt);
   xconfd_get(cce_reg_mapping_interleaved.interlvr_size, enum, "interlvr-size", yt);
   xconfd_get(cce_reg_mapping_interleaved.shift_idx, uint16, "shift-idx", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_coreset__tci_state_info(XCONFD_YANGTREE_T* yt, TciStateInfo& tci_state_info)
{
   xconfd_yang_tree_get_leaf_list(tci_state_info.to_add, uint8, "to-add", yt);
   xconfd_yang_tree_get_leaf_list(tci_state_info.to_rel, uint8, "to-rel", yt);
   xconfd_get_empty_value(tci_state_info.present_in_dci, "present-in-dci", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_pdcch_cfg_cmn__search_spaces(XCONFD_YANGTREE_T* yt, SearchSpacePtrVector& search_spaces)
{
   XCONFD_YANG_TREE_LIST_FOREACH(yt, search_space_yt)
   {
      read_grp_search_space(search_space_yt, search_spaces);
   }
}

void oam_agent_rcfd_cell_schd::read_grp_search_space(XCONFD_YANGTREE_T* yt, SearchSpacePtrVector& search_spaces)
{
   auto search_space = std::make_shared<SearchSpace>();

   xconfd_get(search_space->id, uint8, "id", yt);
   xconfd_get(search_space->coreset_id, uint8, "coreset-id", yt);
   xconfd_get_optional(search_space->mon_slot_perd_offset, uint16_t, uint16, "mon-slot-perd-offset", yt);
   xconfd_get(search_space->mon_slot_syms, uint16, "mon-slot-syms", yt);

   auto aggr_lvl_candidates_num_yt = xconfd_yang_tree_get_node(yt, "aggr-lvl-candidates-num");
   read_grp_search_spaces__aggr_lvl_candidates_num(aggr_lvl_candidates_num_yt, search_space->aggr_lvl_candidates_num);

   xconfd_get_optional_enum(search_space->dci_format, DciFormatE, "dci-format", yt);

   search_spaces.push_back(search_space);
}

void oam_agent_rcfd_cell_schd::read_grp_search_spaces__aggr_lvl_candidates_num(
   XCONFD_YANGTREE_T* yt, AggrLvlCandidatesNum& aggr_lvl_candidates_num)
{
   xconfd_get(aggr_lvl_candidates_num.aggr_lvl1, enum, "aggr-lvl1", yt);
   xconfd_get(aggr_lvl_candidates_num.aggr_lvl2, enum, "aggr-lvl2", yt);
   xconfd_get(aggr_lvl_candidates_num.aggr_lvl4, enum, "aggr-lvl4", yt);
   xconfd_get(aggr_lvl_candidates_num.aggr_lvl8, enum, "aggr-lvl8", yt);
   xconfd_get(aggr_lvl_candidates_num.aggr_lvl16, enum, "aggr-lvl16", yt);
}

void oam_agent_rcfd_cell_schd::read_tdd_cfg_cmn(XCONFD_YANGTREE_T* yt)
{
   if (!yt) return;
   tdd_cfg_cmn_ = std::make_shared<TddCfgCmn>();

   read_grp_tdd_cfg_cmn(yt, *tdd_cfg_cmn_);
}

void oam_agent_rcfd_cell_schd::read_grp_tdd_cfg_cmn(XCONFD_YANGTREE_T* yt, TddCfgCmn& tdd_cfg_cmn)
{
   xconfd_get(tdd_cfg_cmn.ref_scs, enum, "ref-scs", yt);

   auto pattern1_yt = xconfd_yang_tree_get_node(yt, "pattern1");
   read_grp_tdd_cfg_cmn__pattern1(pattern1_yt, tdd_cfg_cmn.pattern1);

   auto pattern2_yt = xconfd_yang_tree_get_node(yt, "pattern2");
   read_grp_tdd_cfg_cmn__pattern2(pattern2_yt, tdd_cfg_cmn.pattern2);
}

void oam_agent_rcfd_cell_schd::read_grp_tdd_cfg_cmn__pattern1(XCONFD_YANGTREE_T* yt, TddPattern& pattern1)
{
   read_grp_tdd_ul_dl_pattern(yt, pattern1);
}

void oam_agent_rcfd_cell_schd::read_grp_tdd_ul_dl_pattern(XCONFD_YANGTREE_T* yt, TddPattern& tdd_ul_dl_pattern)
{
   xconfd_get(tdd_ul_dl_pattern.dl_syms_num, uint8, "dl-syms-num", yt);
   xconfd_get(tdd_ul_dl_pattern.dl_slots_num, uint16, "dl-slots-num", yt);
   xconfd_get(tdd_ul_dl_pattern.ul_syms_num, uint8, "ul-syms-num", yt);
   xconfd_get(tdd_ul_dl_pattern.ul_slots_num, uint16, "ul-slots-num", yt);
   xconfd_get(tdd_ul_dl_pattern.trans_perd, enum, "trans-perd", yt);
   xconfd_get(tdd_ul_dl_pattern.trans_perd_v1530, enum, "trans-perd-v1530", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_tdd_cfg_cmn__pattern2(XCONFD_YANGTREE_T* yt, TddPattern& pattern2)
{
   read_grp_tdd_ul_dl_pattern(yt, pattern2);
}

void oam_agent_rcfd_cell_schd::read_sul_cfg(XCONFD_YANGTREE_T* yt)
{
   if (!yt) return;
   sul_cfg_ = std::make_shared<SulCfg>();

   read_grp_sul_cfg(yt, *sul_cfg_);
}

void oam_agent_rcfd_cell_schd::read_grp_sul_cfg(XCONFD_YANGTREE_T* yt, SulCfg& sul_cfg)
{
   xconfd_get(sul_cfg.max_ue_per_ul_tti, uint32, "max-ue-per-ul-tti", yt);
   xconfd_get(sul_cfg.tar_cqi, uint8, "tar-cqi", yt);

   auto freq_info_yt = xconfd_yang_tree_get_node(yt, "freq-info");
   read_grp_sul_cfg__freq_info(freq_info_yt, sul_cfg.freq_info);

   auto initial_bwp_yt = xconfd_yang_tree_get_node(yt, "initial-bwp");
   read_grp_sul_cfg__initial_bwp(initial_bwp_yt, sul_cfg.initial_bwp);

   auto addtl_bwps_yt = xconfd_yang_tree_get_node(yt, "addtl-bwps");
   read_grp_sul_cfg__addtl_bwps(addtl_bwps_yt, sul_cfg.addtl_bwps);

   xconfd_get_empty_value(sul_cfg.harmonic, "harmonic", yt);
}

void oam_agent_rcfd_cell_schd::read_grp_sul_cfg__freq_info(XCONFD_YANGTREE_T* yt, UlFreqInfo& freq_info)
{
   read_grp_ul_freq_info(yt, freq_info);
}

void oam_agent_rcfd_cell_schd::read_grp_sul_cfg__initial_bwp(XCONFD_YANGTREE_T* yt, UlBwpCmn& initial_bwp)
{
   read_grp_ul_bwp_cmn(yt, initial_bwp);
}

void oam_agent_rcfd_cell_schd::read_grp_sul_cfg__addtl_bwps(XCONFD_YANGTREE_T* yt, UlBwpPtrVector& addtl_bwps)
{
   XCONFD_YANG_TREE_LIST_FOREACH(yt, addtl_bwp_yt)
   {
      read_grp_ul_bwp(addtl_bwp_yt, addtl_bwps);
   }
}

void oam_agent_rcfd_cell_schd::read_grp_ul_bwp(XCONFD_YANGTREE_T* yt, UlBwpPtrVector& ul_bwps)
{
   auto ul_bwp = std::make_shared<UlBwp>();
   
   xconfd_get(ul_bwp->bwp_id, uint8, "bwp-id", yt);
   read_grp_ul_bwp_cmn(yt, ul_bwp->ul_bwp_cmn);

   ul_bwps.push_back(ul_bwp);
}

void oam_agent_rcfd_cell_schd::read_ss_pbch_cfg(XCONFD_YANGTREE_T* yt)
{
   read_grp_ss_pbch_cfg(yt, ss_pbch_cfg_);
}

void oam_agent_rcfd_cell_schd::read_grp_ss_pbch_cfg(XCONFD_YANGTREE_T* yt, SsPbchCfg& ss_pbch_cfg)
{
   xconfd_get(ss_pbch_cfg.ssb_sc_offset, uint8, "ssb-sc-offset", yt);
   xconfd_get(ss_pbch_cfg.burst_set_size, uint8, "burst-set-size", yt);
   xconfd_get(ss_pbch_cfg.ssb_power, int8, "ssb-power", yt);
}

void oam_agent_rcfd_cell_schd::read_addtl_bwp_cfg(XCONFD_YANGTREE_T* yt)
{
   if (!yt) return;
   addtl_bwp_cfg_ = std::make_shared<AddtlBwpCfg>();

   auto ul_bwps_yt = xconfd_yang_tree_get_node(yt, "ul-bwps");
   read_addtl_bwp_cfg__ul_bwps(ul_bwps_yt, addtl_bwp_cfg_->ul_bwps);

   auto dl_bwps_yt = xconfd_yang_tree_get_node(yt, "dl-bwps");
   read_addtl_bwp_cfg__dl_bwps(dl_bwps_yt, addtl_bwp_cfg_->dl_bwps);
}

void oam_agent_rcfd_cell_schd::read_addtl_bwp_cfg__ul_bwps(XCONFD_YANGTREE_T* yt, UlBwpPtrVector& ul_bwps)
{
   XCONFD_YANG_TREE_LIST_FOREACH(yt, ul_bwp_yt)
   {
      read_grp_ul_bwp(ul_bwp_yt, ul_bwps);
   }
}

void oam_agent_rcfd_cell_schd::read_addtl_bwp_cfg__dl_bwps(XCONFD_YANGTREE_T* yt, DlBwpPtrVector& dl_bwps)
{
   XCONFD_YANG_TREE_LIST_FOREACH(yt, dl_bwp_yt)
   {
      read_grp_dl_bwp(dl_bwp_yt, dl_bwps);
   }
}

void oam_agent_rcfd_cell_schd::read_grp_dl_bwp(XCONFD_YANGTREE_T* yt, DlBwpPtrVector& dl_bwps)
{
   auto dl_bwp = std::make_shared<DlBwp>();
   
   xconfd_get(dl_bwp->bwp_id, uint8, "bwp-id", yt);
   read_grp_dl_bwp_cmn(yt, dl_bwp->dl_bwp_cmn);

   dl_bwps.push_back(dl_bwp);
}

void oam_agent_rcfd_cell_schd::read_ue_bsr_cfg(XCONFD_YANGTREE_T* yt)
{
   xconfd_get(ue_bsr_cfg_.perd_bsr_timer, enum, "perd-bsr-timer", yt);
   xconfd_get(ue_bsr_cfg_.retx_bsr_timer, enum, "retx-bsr-timer", yt);
}

int oam_agent_rcfd_cell_schd::freq_domain_res_check(XCONFD_CTX_T* ctx, XCONFD_YANGTREE_T* yt, void* user)
{
   auto freq_domain_res = xconfd_get_uint64(xconfd_yang_tree_get_value_ex(yt, "freq-domain-res"));
   if ((freq_domain_res & 0x07) != 0)
   {
      return xconfd_validate_reply_error(ctx, "BIT STRING(45)");
   }

   return xconfd_validate_reply_ok(ctx);
}

} // namespace rcfd_cell
} // namespace gnb_du

