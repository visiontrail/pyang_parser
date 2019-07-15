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
    CoresetIdT id; 
    uint64_t freq_domain_res; 
    uint8_t duration; 
    std::shared_ptr<CceRegMappingInterleaved> cce_reg_mapping_interleaved; 
    PrecoderGranularityE precoder_granularity; 
    std::shared_ptr<uint16_t> dmrs_scramble_id;
    std::shared_ptr<TciStateInfo> tci_state_info; 
} Coreset;

typedef struct structRachGen
{
    uint8_t prach_cfg_idx; 
    Msg1FdmE msg1_fdm; 
    uint16_t msg1_freq_start; 
    uint8_t zero_corre_zone; 
    int16_t preamble_rx_tpower; 
    PreambleTransMaxE preamble_trans_max; 
    PowerRampStepE power_ramp_step; 
    RaRspWinE ra_rsp_win; 
} RachGen;

typedef struct structFreqInfoCmn
{
    uint32_t abs_freq_point_a; 
    ArfcnT abs_arfcn_point_a; 
    ArfcnT nr_arfcn; 
    ChannelBwE channel_bw; 
    std::vector<uint16_t> nr_freq_bands;
    std::vector<std::shared_ptr<ScsSpecCarrier>> scs_spec_carriers;
} FreqInfoCmn;

typedef struct structRachCfgCmn
{
    RachGen rach_gen; 
    uint8_t total_num_ra_preamble; 
    SsbPerRachOccE ssb_per_rach_occ; 
    uint8_t cb_preamble_per_ssb; 
    std::shared_ptr<RachGrpb> rach_grpb; 
    RaContResTmrE ra_cont_res_tmr; 
    PrachRootSeqIdxTypeE prach_root_seq_idx_type; 
    uint16_t prach_root_seq_idx; 
    ScsE msg1_scs; 
    RestrSetE restr_set; 
    bool msg3_tran_precoding; 
    uint8_t max_msg3_trans; 
    uint8_t rsrp_thres_ssb; 
    uint8_t rsrp_thres_ssb_sul; 
} RachCfgCmn;

typedef struct structUlBwpCmn
{
    std::shared_ptr<BwpGen> bwp_gen; 
    std::shared_ptr<RachCfgCmn> rach_cfg_cmn; 
    std::shared_ptr<PuschCfgCmn> pusch_cfg_cmn; 
    std::shared_ptr<PucchCfgCmn> pucch_cfg_cmn; 
} UlBwpCmn;

typedef struct structUlBwp
{
    BwpIdT bwp_id; 
    UlBwpCmn ul_bwp_cmn; 
} UlBwp;

typedef struct structTddUlDlPattern
{
    SymsNumT dl_syms_num; 
    SlotsNumT dl_slots_num; 
    SymsNumT ul_syms_num; 
    SlotsNumT ul_slots_num; 
    TransPerdE trans_perd; 
    std::shared_ptr<TransPerdV1530E> trans_perd_v1530;
} TddUlDlPattern;

typedef struct structScsSpecCarrier
{
    OffsetPointACarrierT offset_to_carrier; 
    ScsE scs; 
} ScsSpecCarrier;

typedef struct structDlBwpCmn
{
    std::shared_ptr<BwpGen> bwp_gen; 
    std::shared_ptr<PdcchCfgCmn> pdcch_cfg_cmn; 
    bool pdsch_present; 
} DlBwpCmn;

typedef struct structPucchCfgCmn
{
    std::shared_ptr<uint8_t> res_cmn;
    PucchGrpHoppingE grp_hopping; 
    std::shared_ptr<uint16_t> hopping_id;
    std::shared_ptr<int16_t> p0_nominal;
} PucchCfgCmn;

typedef struct structSul
{
    uint32_t max_ue_per_ul_tti; 
    uint8_t tar_cqi; 
    uint8_t ccch_cqi; 
    std::shared_ptr<UlFreqInfo> freq_info; 
    UlBwpCmn initial_bwp; 
    std::vector<std::shared_ptr<UlBwp>> addtl_bwps;
    bool harmonic; 
} Sul;

typedef struct structUlFreqInfo
{
    FreqInfoCmn freq_info_cmn; 
    uint8_t addtl_spectrum_emi; 
    int8_t p_max; 
    bool freq_shift_7p5khz; 
} UlFreqInfo;

typedef struct structDlFreqInfo
{
    FreqInfoCmn freq_info_cmn; 
    std::shared_ptr<uint32_t> abs_freq_ssb;
    std::shared_ptr<ArfcnT> abs_arfcn_ssb;
    OffsetPointACarrierT offset_to_point_a; 
} DlFreqInfo;

typedef struct structDlBwp
{
    BwpIdT bwp_id; 
    DlBwpCmn dl_bwp_cmn; 
} DlBwp;

typedef struct structSearchSpace
{
    SsIdT id; 
    CoresetIdT coreset_id; 
    MonSlotPerdOffsetTypeE mon_slot_perd_offset_type; 
    uint16_t mon_slot_perd_offset; 
    uint16_t mon_slot_syms; 
    AggrLvlCandidatesNum aggr_lvl_candidates_num; 
    std::shared_ptr<DciFormatE> dci_format;
} SearchSpace;

typedef struct structPdcchCfgCmn
{
    std::shared_ptr<Coreset> coreset; 
    std::vector<std::shared_ptr<SearchSpace>> search_spaces;
    std::shared_ptr<SsIdT> ss_sib1;
    std::shared_ptr<SsIdT> ss_other_si;
    std::shared_ptr<SsIdT> ss_paging;
    std::shared_ptr<SsIdT> ss_ra;
    std::shared_ptr<CoresetZeroT> coreset_zero;
    std::shared_ptr<SsZeroT> ss_zero;
} PdcchCfgCmn;

typedef struct structDlCfgCmn
{
    DlFreqInfo freq_info; 
    std::shared_ptr<DlBwpCmn> initial_bwp; 
} DlCfgCmn;

typedef struct structRachGrpb
{
    RaMsg3SizeGrpAE ra_msg3_size_grp_a; 
    uint8_t num_ra_preamble_grp_a; 
    MsgPowerOffsetGrpBE msg_power_offset_grp_b; 
} RachGrpb;

typedef struct structBwpGen
{
    uint16_t start_rb; 
    uint16_t rb_num; 
    ScsE scs; 
    bool cp_extended; 
} BwpGen;

typedef struct structTddCfgCmn
{
    ScsE ref_scs; 
    TddUlDlPattern pattern1; 
    std::shared_ptr<TddUlDlPattern> pattern2; 
} TddCfgCmn;

typedef struct structUlCfgCmn
{
    bool present_in_sib1; 
    std::shared_ptr<UlFreqInfo> freq_info; 
    UlBwpCmn initial_bwp; 
    AlignmentTimerE time_alignment_timer; 
    UeHarqInfo ue_harq_info; 
} UlCfgCmn;

typedef struct structPuschCfgCmn
{
    bool grp_hopping_trans_precoding; 
    std::shared_ptr<int8_t> msg3_delta_preamble;
    std::shared_ptr<int16_t> p0_nominal_with_grant;
} PuschCfgCmn;

typedef struct structSsPbch
{
    ScsCmnE scs_cmn; 
    SsbScOffsetT ssb_sc_offset; 
    uint8_t burst_set_size; 
    SsbScellPerdE ssb_scell_perd; 
    int8_t ssb_power; 
} SsPbch;

typedef struct structCceRegMappingInterleaved
{
    RegBndlSizeE reg_bndl_size; 
    InterlvrSizeE interlvr_size; 
    uint16_t shift_idx; 
} CceRegMappingInterleaved; 

typedef struct structTciStateInfo
{
    std::vector<TciStateIdT> to_add;
    std::vector<TciStateIdT> to_rel;
    bool present_in_dci; 
} TciStateInfo; 

typedef struct structFreqInfo
{
    UlFreqInfo ul_freq_info
} FreqInfo; 

typedef struct structInitialBwp
{
    UlBwpCmn ul_bwp_cmn
} InitialBwp; 

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

typedef struct structAddtlBwps
{
    std::vector<std::shared_ptr<UlBwp>> ul_bwps;
    std::vector<std::shared_ptr<DlBwp>> dl_bwps;
} AddtlBwp; 

typedef struct structUeBsrTimers
{
    std::shared_ptr<PerdBsrTimerE> perd_bsr_timer;
    RetxBsrTimerE retx_bsr_timer; 
} UeBsrTimers; 

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

typedef struct structSearchspaces
{
    SsIdT id; 
    CoresetIdT coreset_id; 
    MonSlotPerdOffsetTypeE mon_slot_perd_offset_type; 
    uint16_t mon_slot_perd_offset; 
    uint16_t mon_slot_syms; 
    AggrLvlCandidatesNum aggr_lvl_candidates_num; 
    DciFormatE dci_format; 
} SearchSpaces; 

typedef struct structScsspeccarriers
{
    OffsetPointACarrierT offset_to_carrier; 
    ScsE scs; 
} ScsSpecCarriers; 

typedef struct structUlbwps
{
    BwpIdT bwp_id; 
    BwpGen bwp_gen; 
    RachCfgCmn rach_cfg_cmn; 
    PuschCfgCmn pusch_cfg_cmn; 
    PucchCfgCmn pucch_cfg_cmn; 
} UlBwps; 

typedef struct structDlbwps
{
    BwpIdT bwp_id; 
    BwpGen bwp_gen; 
    PdcchCfgCmn pdcch_cfg_cmn; 
    bool pdsch_present; 
} DlBwps; 

class oam_agent_rcfd_cell_schd : public allocator
{
public:
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
    UeBsrTimers ue_bsr_timers_; 

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
    void read_ue_bsr_timers(XCONFD_YANGTREE_T* yt);
    void read_grp_coreset(XCONFD_YANGTREE_T* yt, Coreset& coreset);
    void read_grp_coreset__cce_reg_mapping_interleaved(XCONFD_YANGTREE_T* yt, std::shared_ptr<CceRegMappingInterleaved>& cce_reg_mapping_interleaved);
    void read_grp_coreset__tci_state_info(XCONFD_YANGTREE_T* yt, std::shared_ptr<TciStateInfo>& tci_state_info);
    void read_grp_rach_gen(XCONFD_YANGTREE_T* yt, RachGen& rach_gen);
    void read_grp_freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn);
    void read_grp_freq_info_cmn__scs_spec_carriers(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<ScsSpecCarrier>>& scs_spec_carriers);
    void read_grp_rach_cfg_cmn(XCONFD_YANGTREE_T* yt, RachCfgCmn& rach_cfg_cmn);
    void read_grp_rach_cfg_cmn__rach_gen(XCONFD_YANGTREE_T* yt, RachGen& rach_gen);
    void read_grp_rach_cfg_cmn__rach_grpb(XCONFD_YANGTREE_T* yt, std::shared_ptr<RachGrpb>& rach_grpb);
    void read_grp_ul_bwp_cmn(XCONFD_YANGTREE_T* yt, UlBwpCmn& ul_bwp_cmn);
    void read_grp_ul_bwp_cmn__bwp_gen(XCONFD_YANGTREE_T* yt, std::shared_ptr<BwpGen>& bwp_gen);
    void read_grp_ul_bwp_cmn__rach_cfg_cmn(XCONFD_YANGTREE_T* yt, std::shared_ptr<RachCfgCmn>& rach_cfg_cmn);
    void read_grp_ul_bwp_cmn__pusch_cfg_cmn(XCONFD_YANGTREE_T* yt, std::shared_ptr<PuschCfgCmn>& pusch_cfg_cmn);
    void read_grp_ul_bwp_cmn__pucch_cfg_cmn(XCONFD_YANGTREE_T* yt, std::shared_ptr<PucchCfgCmn>& pucch_cfg_cmn);
    void read_grp_ul_bwp(XCONFD_YANGTREE_T* yt, UlBwp& ul_bwp);
    void read_grp_ul_bwp__bwp_gen(XCONFD_YANGTREE_T* yt, std::shared_ptr<BwpGen>& bwp_gen);
    void read_grp_ul_bwp__rach_cfg_cmn(XCONFD_YANGTREE_T* yt, std::shared_ptr<RachCfgCmn>& rach_cfg_cmn);
    void read_grp_ul_bwp__pusch_cfg_cmn(XCONFD_YANGTREE_T* yt, std::shared_ptr<PuschCfgCmn>& pusch_cfg_cmn);
    void read_grp_ul_bwp__pucch_cfg_cmn(XCONFD_YANGTREE_T* yt, std::shared_ptr<PucchCfgCmn>& pucch_cfg_cmn);
    void read_grp_tdd_ul_dl_pattern(XCONFD_YANGTREE_T* yt, TddUlDlPattern& tdd_ul_dl_pattern);
    void read_grp_scs_spec_carrier(XCONFD_YANGTREE_T* yt, ScsSpecCarrier& scs_spec_carrier);
    void read_grp_dl_bwp_cmn(XCONFD_YANGTREE_T* yt, DlBwpCmn& dl_bwp_cmn);
    void read_grp_dl_bwp_cmn__bwp_gen(XCONFD_YANGTREE_T* yt, std::shared_ptr<BwpGen>& bwp_gen);
    void read_grp_dl_bwp_cmn__pdcch_cfg_cmn(XCONFD_YANGTREE_T* yt, std::shared_ptr<PdcchCfgCmn>& pdcch_cfg_cmn);
    void read_grp_pucch_cfg_cmn(XCONFD_YANGTREE_T* yt, PucchCfgCmn& pucch_cfg_cmn);
    void read_grp_sul(XCONFD_YANGTREE_T* yt, Sul& sul);
    void read_grp_sul__freq_info(XCONFD_YANGTREE_T* yt, std::shared_ptr<UlFreqInfo>& freq_info);
    void read_grp_sul__initial_bwp(XCONFD_YANGTREE_T* yt, UlBwpCmn& initial_bwp);
    void read_grp_sul__addtl_bwps(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<UlBwp>>& addtl_bwps);
    void read_grp_ul_freq_info(XCONFD_YANGTREE_T* yt, UlFreqInfo& ul_freq_info);
    void read_grp_ul_freq_info__freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn);
    void read_grp_dl_freq_info(XCONFD_YANGTREE_T* yt, DlFreqInfo& dl_freq_info);
    void read_grp_dl_freq_info__freq_info_cmn(XCONFD_YANGTREE_T* yt, FreqInfoCmn& freq_info_cmn);
    void read_grp_dl_bwp(XCONFD_YANGTREE_T* yt, DlBwp& dl_bwp);
    void read_grp_dl_bwp__bwp_gen(XCONFD_YANGTREE_T* yt, std::shared_ptr<BwpGen>& bwp_gen);
    void read_grp_dl_bwp__pdcch_cfg_cmn(XCONFD_YANGTREE_T* yt, std::shared_ptr<PdcchCfgCmn>& pdcch_cfg_cmn);
    void read_grp_search_space(XCONFD_YANGTREE_T* yt, SearchSpace& search_space);
    void read_grp_search_space__aggr_lvl_candidates_num(XCONFD_YANGTREE_T* yt, AggrLvlCandidatesNum& aggr_lvl_candidates_num);
    void read_grp_pdcch_cfg_cmn(XCONFD_YANGTREE_T* yt, PdcchCfgCmn& pdcch_cfg_cmn);
    void read_grp_pdcch_cfg_cmn__coreset(XCONFD_YANGTREE_T* yt, std::shared_ptr<Coreset>& coreset);
    void read_grp_pdcch_cfg_cmn__search_spaces(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<SearchSpace>>& search_spaces);
    void read_grp_dl_cfg_cmn(XCONFD_YANGTREE_T* yt, DlCfgCmn& dl_cfg_cmn);
    void read_grp_dl_cfg_cmn__freq_info(XCONFD_YANGTREE_T* yt, DlFreqInfo& freq_info);
    void read_grp_dl_cfg_cmn__initial_bwp(XCONFD_YANGTREE_T* yt, std::shared_ptr<DlBwpCmn>& initial_bwp);
    void read_grp_rach_grpb(XCONFD_YANGTREE_T* yt, RachGrpb& rach_grpb);
    void read_grp_bwp_gen(XCONFD_YANGTREE_T* yt, BwpGen& bwp_gen);
    void read_grp_tdd_cfg_cmn(XCONFD_YANGTREE_T* yt, TddCfgCmn& tdd_cfg_cmn);
    void read_grp_tdd_cfg_cmn__pattern1(XCONFD_YANGTREE_T* yt, TddUlDlPattern& pattern1);
    void read_grp_tdd_cfg_cmn__pattern2(XCONFD_YANGTREE_T* yt, std::shared_ptr<TddUlDlPattern>& pattern2);
    void read_grp_ul_cfg_cmn(XCONFD_YANGTREE_T* yt, UlCfgCmn& ul_cfg_cmn);
    void read_grp_ul_cfg_cmn__freq_info(XCONFD_YANGTREE_T* yt, std::shared_ptr<UlFreqInfo>& freq_info);
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