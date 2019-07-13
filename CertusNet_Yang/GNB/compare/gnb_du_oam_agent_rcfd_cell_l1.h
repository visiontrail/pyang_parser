/*
 * filename: gnb_du_oam_agent_cell_l1.h 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#ifndef __GNB_DU_OAM_AGENT_CELL_L1__
#define __GNB_DU_OAM_AGENT_CELL_L1__

#include "gnb_du_oam_agent_rcfd_cell_types.h"

namespace gnb_du 
{
namespace rcfd
{

typedef struct structPlmnIdInfo
{
    std::vector<PlmnIdPtr> plmn_ids;
    NrCellIdT nr_cell_id; 
    std::shared_ptr<uint32_t> tac; 
    std::shared_ptr<uint8_t> ranac; 
    bool cell_rsrvd_for_operator_use; 
} PlmnIdInfo;

typedef struct structCellSelInfo
{
    std::shared_ptr<QRxLvlMinT> q_rx_lvl_min; 
    std::shared_ptr<uint8_t> q_rx_lvl_min_offset; 
    std::shared_ptr<QRxLvlMinT> q_rx_lvl_min_sul; 
    std::shared_ptr<QQualMinT> q_qual_min; 
    std::shared_ptr<uint8_t> q_qual_min_offset; 
} CellSelInfo;

typedef std::shared_ptr<PlmnIdInfo> PlmnIdInfoPtr;
typedef std::vector<PlmnIdInfoPtr> PlmnIdInfoPtrVector;

typedef struct structCellAccessInfo
{
    PlmnIdInfoPtrVector plmn_id_infos;
    bool cell_rsrvd_for_other_use; 
} CellAccessInfo;

typedef struct structUeTimers
{
    T300E t300; 
    T301E t301; 
    T310E t310; 
    N310E n310; 
    T311E t311; 
    N311E n311; 
    T319E t319; 
} UeTimers;

typedef struct structPrach
{
    uint16_t root_seq_idx; 
    uint8_t cfg_idx; 
    uint8_t zero_correlation_zone_cfg; 
    std::shared_ptr<uint8_t> restricted_set; 
    uint16_t freq_start; 
    uint8_t freq_offset; 
    ScsE scs; 
    Msg1FdmE fdm; 
    SsbPerRachOccE ssb_per_rach_occ; 
} Prach;

typedef struct structPdcchCfgSib1
{
    CoresetZeroT coreset_zero; 
    SsZeroT ss_zero; 
} PdcchCfgSib1; 

typedef struct structMib
{
    ScsCmnE scs_cmn; 
    SsbScOffsetT ssb_sc_offset; 
    PdcchCfgSib1 pdcch_cfg_sib1; 
    bool cell_barred; 
    bool intra_freq_resel; 
} Mib;

typedef struct structSib1
{
    CellSelInfo cell_sel_info; 
    CellAccessInfo cell_access_info; 
    UeTimers ue_timers; 
} Sib1;

class oam_agent_rcfd_cell_l1 : public allocator
{
public:
    uint32_t dl_central_freq_; 
    uint32_t ul_central_freq_; 
    uint8_t ca_lvl_; 
    std::shared_ptr<Prach> prach_;
    Mib mib_; 
    Sib1 sib1_; 

    void read_prach(XCONFD_YANGTREE_T* yt);
    void read_mib(XCONFD_YANGTREE_T* yt);
    void read_sib1(XCONFD_YANGTREE_T* yt);
    void read_grp_cell_access_info(XCONFD_YANGTREE_T* yt, CellAccessInfo& cell_access_info);
    void read_grp_cell_access_info__plmn_id_infos(XCONFD_YANGTREE_T* yt, PlmnIdInfoPtrVector& plmn_id_infos);
    void read_grp_plmn_id_info(XCONFD_YANGTREE_T* yt, PlmnIdInfo& plmn_id_info);
    void read_grp_plmn_id_info__plmn_ids(XCONFD_YANGTREE_T* yt, PlmnIdPtrVector& plmn_ids);
    void read_grp_sib1(XCONFD_YANGTREE_T* yt, Sib1& sib1);
    void read_grp_sib1__cell_sel_info(XCONFD_YANGTREE_T* yt, CellSelInfo& cell_sel_info);
    void read_grp_sib1__cell_access_info(XCONFD_YANGTREE_T* yt, CellAccessInfo& cell_access_info);
    void read_grp_sib1__ue_timers(XCONFD_YANGTREE_T* yt, UeTimers& ue_timers);
    void read_grp_mib(XCONFD_YANGTREE_T* yt, Mib& mib);
    void read_grp_mib__pdcch_cfg_sib1(XCONFD_YANGTREE_T* yt, PdcchCfgSib1& pdcch_cfg_sib1);
    void read_grp_ue_timers(XCONFD_YANGTREE_T* yt, UeTimers& ue_timers);
    void read_grp_cell_sel_info(XCONFD_YANGTREE_T* yt, CellSelInfo& cell_sel_info);
    void read_grp_prach(XCONFD_YANGTREE_T* yt, Prach& prach);

public:
    oam_agent_rcfd_cell_l1(XCONFD_YANGTREE_T* yt);
    virtual ~oam_agent_rcfd_cell_l1() {}
};

} // namespace rcfd
} // namespace gnb_du
#endif /* __GNB_DU_OAM_AGENT_CELL_L1__ */