/*
 * filename: gnb_du_oam_agent_cell_schd.h 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#ifndef __GNB_DU_OAM_AGENT_CELL_SCHD__
#define __GNB_DU_OAM_AGENT_CELL_SCHD__

#include "gnb_du_oam_agent_rcfd_cell_types.h"

namespace gnb_du 
{
namespace rcfd
{

typedef struct structCoreset
{
    id id; 
    freq-domain-res freq_domain_res; 
    duration duration; 
    CceRegMappingInterleaved cce_reg_mapping_interleaved; 
    precoder-granularity precoder_granularity; 
    dmrs-scramble-id dmrs_scramble_id; 
    TciStateInfo tci_state_info; 
} Coreset;

typedef struct structRachGen
{
    prach-cfg-idx prach_cfg_idx; 
    msg1-fdm msg1_fdm; 
    msg1-freq-start msg1_freq_start; 
    zero-corre-zone zero_corre_zone; 
    preamble-rx-tpower preamble_rx_tpower; 
    preamble-trans-max preamble_trans_max; 
    power-ramp-step power_ramp_step; 
    ra-rsp-win ra_rsp_win; 
} RachGen;

typedef struct structFreqInfoCmn
{
    abs-freq-point-a abs_freq_point_a; 
    abs-arfcn-point-a abs_arfcn_point_a; 
    nr-arfcn nr_arfcn; 
    channel-bw channel_bw; 
    std::vector<std::shared_ptr<uint16_t>> nr_freq_bands;
    std::vector<std::shared_ptr<ScsSpecCarrier>> scs_spec_carriers;
} FreqInfoCmn;

typedef struct structRachCfgCmn
{
    RachGen rach_gen; 
    total-num-ra-preamble total_num_ra_preamble; 
    ssb-per-rach-occ ssb_per_rach_occ; 
    cb-preamble-per-ssb cb_preamble_per_ssb; 
    RachGrpb rach_grpb; 
    ra-cont-res-tmr ra_cont_res_tmr; 
    prach-root-seq-idx-type prach_root_seq_idx_type; 
    prach-root-seq-idx prach_root_seq_idx; 
    msg1-scs msg1_scs; 
    restr-set restr_set; 
    msg3-tran-precoding msg3_tran_precoding; 
    max-msg3-trans max_msg3_trans; 
    rsrp-thres-ssb rsrp_thres_ssb; 
    rsrp-thres-ssb-sul rsrp_thres_ssb_sul; 
} RachCfgCmn;

typedef struct structUlBwpCmn
{
    BwpGen bwp_gen; 
    RachCfgCmn rach_cfg_cmn; 
    PuschCfgCmn pusch_cfg_cmn; 
    PucchCfgCmn pucch_cfg_cmn; 
} UlBwpCmn;

typedef struct structUlBwp
{
    ul-bwp-cmn ul_bwp_cmn; 
} UlBwp;

typedef struct structTddUlDlPattern
{
    dl-syms-num dl_syms_num; 
    dl-slots-num dl_slots_num; 
    ul-syms-num ul_syms_num; 
    ul-slots-num ul_slots_num; 
    trans-perd trans_perd; 
    trans-perd-v1530 trans_perd_v1530; 
} TddUlDlPattern;

typedef struct structScsSpecCarrier
{
    offset-to-carrier offset_to_carrier; 
    scs scs; 
} ScsSpecCarrier;

typedef struct structDlBwpCmn
{
    BwpGen bwp_gen; 
    PdcchCfgCmn pdcch_cfg_cmn; 
    pdsch-present pdsch_present; 
} DlBwpCmn;

typedef struct structPucchCfgCmn
{
    res-cmn res_cmn; 
    grp-hopping grp_hopping; 
    hopping-id hopping_id; 
    p0-nominal p0_nominal; 
} PucchCfgCmn;

typedef struct structSul
{
    max-ue-per-ul-tti max_ue_per_ul_tti; 
    tar-cqi tar_cqi; 
    ccch-cqi ccch_cqi; 
    FreqInfo freq_info; 
    InitialBwp initial_bwp; 
    std::vector<std::shared_ptr<AddtlBwp>> addtl_bwps;
    harmonic harmonic; 
} Sul;

typedef struct structUlFreqInfo
{
    FreqInfoCmn freq_info_cmn; 
    addtl-spectrum-emi addtl_spectrum_emi; 
    p-max p_max; 
    freq-shift-7p5khz freq_shift_7p5khz; 
} UlFreqInfo;

typedef struct structDlFreqInfo
{
    FreqInfoCmn freq_info_cmn; 
    abs-freq-ssb abs_freq_ssb; 
    abs-arfcn-ssb abs_arfcn_ssb; 
    offset-to-point-a offset_to_point_a; 
} DlFreqInfo;

typedef struct structDlBwp
{
    dl-bwp-cmn dl_bwp_cmn; 
} DlBwp;

typedef struct structSearchSpace
{
    id id; 
    coreset-id coreset_id; 
    mon-slot-perd-offset-type mon_slot_perd_offset_type; 
    mon-slot-perd-offset mon_slot_perd_offset; 
    mon-slot-syms mon_slot_syms; 
    AggrLvlCandidatesNum aggr_lvl_candidates_num; 
    dci-format dci_format; 
} SearchSpace;

typedef struct structCellSchd
{
    max-ue-per-ul-tti max_ue_per_ul_tti; 
    max-ue-per-dl-tti max_ue_per_dl_tti; 
    max-dl-harq-tx max_dl_harq_tx; 
    max-msg4-harq-tx max_msg4_harq_tx; 
    duration duration; 
    cmn-lcs-num cmn_lcs_num; 
    cp-ul-extended cp_ul_extended; 
    scs scs; 
    dmrs-type-a-pos dmrs_type_a_pos; 
    slot-format slot_format; 
    harq-ack-cb-type harq_ack_cb_type; 
    bwp-inactive-timer bwp_inactive_timer; 
    scell-cmn-cfg-in-sib1 scell_cmn_cfg_in_sib1; 
    PreambleIdRange preamble_id_range; 
    FddCfgCmn fdd_cfg_cmn; 
    TddCfgCmn tdd_cfg_cmn; 
    Sul sul; 
    SsPbch ss_pbch; 
    AddtlBwps addtl_bwps; 
    UeBsrTimer ue_bsr_timer; 
} CellSchd;

typedef struct structPdcchCfgCmn
{
    Coreset coreset; 
    std::vector<std::shared_ptr<SearchSpace>> search_spaces;
    ss-sib1 ss_sib1; 
    ss-other-si ss_other_si; 
    ss-paging ss_paging; 
    ss-ra ss_ra; 
    coreset-zero coreset_zero; 
    ss-zero ss_zero; 
} PdcchCfgCmn;

typedef struct structDlCfgCmn
{
    FreqInfo freq_info; 
    InitialBwp initial_bwp; 
} DlCfgCmn;

typedef struct structRachGrpb
{
    ra-msg3-size-grp-a ra_msg3_size_grp_a; 
    num-ra-preamble-grp-a num_ra_preamble_grp_a; 
    msg-power-offset-grp-b msg_power_offset_grp_b; 
} RachGrpb;

typedef struct structBwpGen
{
    start-rb start_rb; 
    rb-num rb_num; 
    scs scs; 
    cp-extended cp_extended; 
} BwpGen;

typedef struct structTddCfgCmn
{
    ref-scs ref_scs; 
    Pattern1 pattern1; 
    Pattern2 pattern2; 
} TddCfgCmn;

typedef struct structUlCfgCmn
{
    present-in-sib1 present_in_sib1; 
    FreqInfo freq_info; 
    InitialBwp initial_bwp; 
    time-alignment-timer time_alignment_timer; 
    UeHarqInfo ue_harq_info; 
} UlCfgCmn;

typedef struct structPuschCfgCmn
{
    grp-hopping-trans-precoding grp_hopping_trans_precoding; 
    msg3-delta-preamble msg3_delta_preamble; 
    p0-nominal-with-grant p0_nominal_with_grant; 
} PuschCfgCmn;

typedef struct structSsPbch
{
    scs scs; 
    ssb-sc-offset ssb_sc_offset; 
    burst-set-size burst_set_size; 
    ssb-scell-perd ssb_scell_perd; 
    ssb-power ssb_power; 
} SsPbch;

typedef struct structCceRegMappingInterleaved
{
    RegBndlSizeE reg_bndl_size; 
    reg-bndl-size-e interlvr_size; 
    uint16_t shift_idx; 
} CceRegMappingInterleaved; 

typedef struct structTciStateInfo
{
    std::vector<ToAdd> to_add;
    std::vector<ToRel> to_rel;
    bool present_in_dci; 
} TciStateInfo; 

typedef struct structScsSpecCarriers
{
    ScsSpecCarrier scs_spec_carrier
} ScsSpecCarriers; 

typedef struct structFreqInfo
{
    UlFreqInfo ul_freq_info
} FreqInfo; 

typedef struct structInitialBwp
{
    UlBwpCmn ul_bwp_cmn
} InitialBwp; 

typedef struct structAddtlBwps
{
    UlBwp ul_bwp
} AddtlBwps; 

typedef struct structAggrLvlCandidatesNum
{
    CandidatesNumE aggr_lvl1; 
    CandidatesNumE aggr_lvl2; 
    CandidatesNumE aggr_lvl4; 
    CandidatesNumE aggr_lvl8; 
    CandidatesNumE aggr_lvl16; 
} AggrLvlCandidatesNum; 

typedef struct structPreambleIdRange
{
    uint16_t start; 
    uint16_t size; 
} PreambleIdRange; 

typedef struct structFddCfgCmn
{
    UlCfgCmn ul_cfg_cmn; 
    DlCfgCmn dl_cfg_cmn; 
} FddCfgCmn; 

typedef struct structUeBsrTimer
{
    PerdBsrTimerE perd_bsr_timer; 
    RetxBsrTimerE retx_bsr_timer; 
} UeBsrTimer; 

typedef struct structSearchSpaces
{
    SearchSpace search_space
} SearchSpaces; 

typedef struct structPattern1
{
    TddUlDlPattern tdd_ul_dl_pattern
} Pattern1; 

typedef struct structPattern2
{
    TddUlDlPattern tdd_ul_dl_pattern
} Pattern2; 

typedef struct structUeHarqInfo
{
    uint8_t max_harq; 
    uint8_t delta_harq_offset; 
} UeHarqInfo; 

typedef struct structUlbwps
{
    uint8_t bwp_id; 
    BwpGen bwp_gen; 
    RachCfgCmn rach_cfg_cmn; 
    PuschCfgCmn pusch_cfg_cmn; 
    PucchCfgCmn pucch_cfg_cmn; 
} UlBwps; 

typedef struct structDlbwps
{
    uint8_t bwp_id; 
    BwpGen bwp_gen; 
    PdcchCfgCmn pdcch_cfg_cmn; 
    bool pdsch_present; 
} DlBwps; 

class oam_agent_rcfd_cell_schd : public allocator
{
private:
    uint32_t max_ue_per_ul_tti_; 
    uint32_t max_ue_per_dl_tti_; 
    uint8_t max_dl_harq_tx_; 
    uint32_t max_msg4_harq_tx_; 
    uint16_t duration_; 
    uint8_t cmn_lcs_num_; 
    bool cp_ul_extended_; 
    ScsE scs_; 
    DmrsTypeAPosE dmrs_type_a_pos_; 
    uint8_t slot_format_; 
    HarqAckCbTypeE harq_ack_cb_type_; 
    BwpInactiveTimerE bwp_inactive_timer_; 
    bool scell_cmn_cfg_in_sib1_; 
    PreambleIdRange preamble_id_range_; 
    std::shared_ptr<FddCfgCmn> fdd_cfg_cmn_;
    std::shared_ptr<TddCfgCmn> tdd_cfg_cmn_;
    std::shared_ptr<Sul> sul_;
    SsPbch ss_pbch_; 
    std::shared_ptr<AddtlBwps> addtl_bwps_;
    UeBsrTimer ue_bsr_timer_; 

    void read_preamble_id_range(XCONFD_YANGTREE_T* yt);
    void read_fdd_cfg_cmn(XCONFD_YANGTREE_T* yt);
    void read_fdd_cfg_cmn__ul_cfg_cmn(XCONFD_YANGTREE_T* yt, UlCfgCmn& ul_cfg_cmn);
    void read_fdd_cfg_cmn__dl_cfg_cmn(XCONFD_YANGTREE_T* yt, DlCfgCmn& dl_cfg_cmn);
    void read_tdd_cfg_cmn(XCONFD_YANGTREE_T* yt);
    void read_sul(XCONFD_YANGTREE_T* yt);
    void read_ss_pbch(XCONFD_YANGTREE_T* yt);
    void read_addtl_bwps(XCONFD_YANGTREE_T* yt);
    void read_addtl_bwps__ul_bwps(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<UlBwp>>& ul_bwps);
    void read_addtl_bwps__dl_bwps(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<DlBwp>>& dl_bwps);
    void read_ue_bsr_timer(XCONFD_YANGTREE_T* yt);
    void read_grp_coreset(XCONFD_YANGTREE_T* yt, Coreset& coreset);
    void read_grp_coreset__cce_reg_mapping_interleaved(XCONFD_YANGTREE_T* yt, CceRegMappingInterleaved& cce_reg_mapping_interleaved);
    void read_grp_coreset__tci_state_info(XCONFD_YANGTREE_T* yt, TciStateInfo& tci_state_info);
    void read_grp_rach_gen(XCONFD_YANGTREE_T* yt, RachGen& rach_gen);
    void read_grp_freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn);
    void read_grp_freq_info_cmn__scs_spec_carriers(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<ScsSpecCarrier>>& scs_spec_carriers);
    void read_grp_rach_cfg_cmn(XCONFD_YANGTREE_T* yt, RachCfgCmn& rach_cfg_cmn);
    void read_grp_rach_cfg_cmn__rach_gen(XCONFD_YANGTREE_T* yt, RachGen& rach_gen);
    void read_grp_rach_cfg_cmn__rach_grpb(XCONFD_YANGTREE_T* yt, RachGrpb& rach_grpb);
    void read_grp_ul_bwp_cmn(XCONFD_YANGTREE_T* yt, UlBwpCmn& ul_bwp_cmn);
    void read_grp_ul_bwp_cmn__bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen);
    void read_grp_ul_bwp_cmn__rach_cfg_cmn(XCONFD_YANGTREE_T* yt, RachCfgCmn& rach_cfg_cmn);
    void read_grp_ul_bwp_cmn__pusch_cfg_cmn(XCONFD_YANGTREE_T* yt, PuschCfgCmn& pusch_cfg_cmn);
    void read_grp_ul_bwp_cmn__pucch_cfg_cmn(XCONFD_YANGTREE_T* yt, PucchCfgCmn& pucch_cfg_cmn);
    void read_grp_ul_bwp(XCONFD_YANGTREE_T* yt, UlBwp& ul_bwp);
    void read_grp_ul_bwp__bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen);
    void read_grp_ul_bwp__rach_cfg_cmn(XCONFD_YANGTREE_T* yt, RachCfgCmn& rach_cfg_cmn);
    void read_grp_ul_bwp__pusch_cfg_cmn(XCONFD_YANGTREE_T* yt, PuschCfgCmn& pusch_cfg_cmn);
    void read_grp_ul_bwp__pucch_cfg_cmn(XCONFD_YANGTREE_T* yt, PucchCfgCmn& pucch_cfg_cmn);
    void read_grp_tdd_ul_dl_pattern(XCONFD_YANGTREE_T* yt, TddUlDlPattern& tdd_ul_dl_pattern);
    void read_grp_scs_spec_carrier(XCONFD_YANGTREE_T* yt, ScsSpecCarrier& scs_spec_carrier);
    void read_grp_dl_bwp_cmn(XCONFD_YANGTREE_T* yt, DlBwpCmn& dl_bwp_cmn);
    void read_grp_dl_bwp_cmn__bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen);
    void read_grp_dl_bwp_cmn__pdcch_cfg_cmn(XCONFD_YANGTREE_T* yt, PdcchCfgCmn& pdcch_cfg_cmn);
    void read_grp_pucch_cfg_cmn(XCONFD_YANGTREE_T* yt, PucchCfgCmn& pucch_cfg_cmn);
    void read_grp_sul(XCONFD_YANGTREE_T* yt, Sul& sul);
    void read_grp_sul__freq_info(XCONFD_YANGTREE_T* yt, UlFreqInfo& freq_info);
    void read_grp_sul__initial_bwp(XCONFD_YANGTREE_T* yt, UlBwpCmn& initial_bwp);
    void read_grp_sul__addtl_bwps(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<UlBwp>>& addtl_bwps);
    void read_grp_ul_freq_info(XCONFD_YANGTREE_T* yt, UlFreqInfo& ul_freq_info);
    void read_grp_ul_freq_info__freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn);
    void read_grp_dl_freq_info(XCONFD_YANGTREE_T* yt, DlFreqInfo& dl_freq_info);
    void read_grp_dl_freq_info__freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn);
    void read_grp_dl_bwp(XCONFD_YANGTREE_T* yt, DlBwp& dl_bwp);
    void read_grp_dl_bwp__bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen);
    void read_grp_dl_bwp__pdcch_cfg_cmn(XCONFD_YANGTREE_T* yt, PdcchCfgCmn& pdcch_cfg_cmn);
    void read_grp_search_space(XCONFD_YANGTREE_T* yt, SearchSpace& search_space);
    void read_grp_search_space__aggr_lvl_candidates_num(XCONFD_YANGTREE_T* yt, AggrLvlCandidatesNum& aggr_lvl_candidates_num);
    void read_grp_pdcch_cfg_cmn(XCONFD_YANGTREE_T* yt, PdcchCfgCmn& pdcch_cfg_cmn);
    void read_grp_pdcch_cfg_cmn__coreset(XCONFD_YANGTREE_T* yt, Coreset& coreset);
    void read_grp_pdcch_cfg_cmn__search_spaces(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<SearchSpace>>& search_spaces);
    void read_grp_dl_cfg_cmn(XCONFD_YANGTREE_T* yt, DlCfgCmn& dl_cfg_cmn);
    void read_grp_dl_cfg_cmn__freq_info(XCONFD_YANGTREE_T* yt, DlFreqInfo& freq_info);
    void read_grp_dl_cfg_cmn__initial_bwp(XCONFD_YANGTREE_T* yt, DlBwpCmn& initial_bwp);
    void read_grp_rach_grpb(XCONFD_YANGTREE_T* yt, RachGrpb& rach_grpb);
    void read_grp_bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen);
    void read_grp_tdd_cfg_cmn(XCONFD_YANGTREE_T* yt, TddCfgCmn& tdd_cfg_cmn);
    void read_grp_tdd_cfg_cmn__pattern1(XCONFD_YANGTREE_T* yt, TddUlDlPattern& pattern1);
    void read_grp_tdd_cfg_cmn__pattern2(XCONFD_YANGTREE_T* yt, TddUlDlPattern& pattern2);
    void read_grp_ul_cfg_cmn(XCONFD_YANGTREE_T* yt, UlCfgCmn& ul_cfg_cmn);
    void read_grp_ul_cfg_cmn__freq_info(XCONFD_YANGTREE_T* yt, UlFreqInfo& freq_info);
    void read_grp_ul_cfg_cmn__initial_bwp(XCONFD_YANGTREE_T* yt, UlBwpCmn& initial_bwp);
    void read_grp_ul_cfg_cmn__ue_harq_info(XCONFD_YANGTREE_T* yt, UeHarqInfo& ue_harq_info);
    void read_grp_pusch_cfg_cmn(XCONFD_YANGTREE_T* yt, PuschCfgCmn& pusch_cfg_cmn);
    void read_grp_ss_pbch(XCONFD_YANGTREE_T* yt, SsPbch& ss_pbch);

public:
    oam_agent_rcfd_cell_schd(XCONFD_YANGTREE_T* yt);
    virtual ~oam_agent_rcfd_cell_schd() {}
};

} //end of namespace rcfd
} //end of namespace gnb_du
#endif /* __GNB_DU_OAM_AGENT_CELL_SCHD__ */