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

typedef struct structCellAccessInfo
{
    std::vector<std::shared_ptr<PlmnIdInfo>> plmn_id_infos;
    cell-rsrvd-for-other-use cell_rsrvd_for_other_use; 
} CellAccessInfo;

typedef struct structPlmnIdInfo
{
    std::vector<std::shared_ptr<PlmnId>> plmn_ids;
    cell-id cell_id; 
    tac tac; 
    ranac ranac; 
    cell-rsrvd-for-operator-use cell_rsrvd_for_operator_use; 
} PlmnIdInfo;

typedef struct structSib1
{
    CellSelInfo cell_sel_info; 
    CellAccessInfo cell_access_info; 
    UeTimers ue_timers; 
} Sib1;

typedef struct structMib
{
    scs-cmn scs_cmn; 
    ssb-sc-offset ssb_sc_offset; 
    PdcchCfgSib1 pdcch_cfg_sib1; 
    cell-barred cell_barred; 
    intra-freq-resel intra_freq_resel; 
} Mib;

typedef struct structUeTimers
{
    t300 t300; 
    t301 t301; 
    t310 t310; 
    n310 n310; 
    t311 t311; 
    n311 n311; 
    t319 t319; 
} UeTimers;

typedef struct structCellSelInfo
{
    q-rx-lvl-min q_rx_lvl_min; 
    q-rx-lvl-min-offset q_rx_lvl_min_offset; 
    q-rx-lvl-min-sul q_rx_lvl_min_sul; 
    q-qual-min q_qual_min; 
    q-qual-min-offset q_qual_min_offset; 
} CellSelInfo;

typedef struct structPrach
{
    root-seq-idx root_seq_idx; 
    cfg-idx cfg_idx; 
    zero-correlation-zone-cfg zero_correlation_zone_cfg; 
    restricted-set restricted_set; 
    freq-start freq_start; 
    freq-offset freq_offset; 
    scs scs; 
    fdm fdm; 
    ssb-per-rach-occ ssb_per_rach_occ; 
} Prach;

typedef struct structCellL1
{
    dl-central-freq dl_central_freq; 
    ul-central-freq ul_central_freq; 
    ca-lvl ca_lvl; 
    Prach prach; 
    Mib mib; 
    Sib1 sib1; 
} CellL1;

typedef struct structPlmnIdInfos
{
    PlmnIdInfo plmn_id_info
} PlmnIdInfos; 

typedef struct structPlmnIds
{
    PlmnId plmn_id;
} PlmnIds; 

typedef struct structPdcchCfgSib1
{
    uint8_t coreset_zero; 
    uint8_t ss_zero; 
} PdcchCfgSib1; 

class oam_agent_rcfd_cell_l1 : public allocator
{
private:
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
    void read_grp_cell_access_info__plmn_id_infos(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<PlmnIdInfo>>& plmn_id_infos);
    void read_grp_plmn_id_info(XCONFD_YANGTREE_T* yt, PlmnIdInfo& plmn_id_info);
    void read_grp_plmn_id_info__plmn_ids(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<PlmnId>>& plmn_ids);
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

} //end of namespace rcfd
} //end of namespace gnb_du
#endif /* __GNB_DU_OAM_AGENT_CELL_L1__ */