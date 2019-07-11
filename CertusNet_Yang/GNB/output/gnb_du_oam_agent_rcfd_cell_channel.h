/*
 * filename: gnb_du_oam_agent_cell_channel.h 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#ifndef __GNB_DU_OAM_AGENT_CELL_CHANNEL__
#define __GNB_DU_OAM_AGENT_CELL_CHANNEL__

#include "gnb_du_oam_agent_rcfd_cell_types.h"

namespace gnb_du 
{
namespace rcfd
{

typedef struct structCellChannel
{
    Pcch pcch; 
    Bcch bcch; 
    PuschScell pusch_scell; 
    PdschScell pdsch_scell; 
} CellChannel;

typedef struct structPcch
{
    PagingCycleE default_paging_cycle; 
    PagingFrameOffsetTypeE paging_frame_offset_type; 
    uint8_t paging_frame_offset; 
    NsE ns; 
    bool f_pdcch_mon_occ_of_po_present; 
} Pcch; 

typedef struct structBcch
{
    ModiPerdCoeffE modi_perd_coeff; 
    SiWinLenE si_win_len; 
} Bcch; 

typedef struct structPuschScell
{
    MaxCbgsPerTbE max_cbgs_per_tb; 
    bool limited_buf_rm; 
    XOverheadE x_overhead; 
} PuschScell; 

typedef struct structPdschScell
{
    MaxCbgsPerTbE max_cbgs_per_tb; 
    bool cbg_flush_ind; 
    XOverheadE x_overhead; 
    PdschHarqProcessesNumE harq_processes_num; 
    uint8_t pucch_cell_idx; 
} PdschScell; 

class oam_agent_rcfd_cell_channel : public allocator
{
private:
    Pcch pcch_; 
    Bcch bcch_; 
    PuschScell pusch_scell_; 
    PdschScell pdsch_scell_; 

    void read_pcch(XCONFD_YANGTREE_T* yt);
    void read_bcch(XCONFD_YANGTREE_T* yt);
    void read_pusch_scell(XCONFD_YANGTREE_T* yt);
    void read_pdsch_scell(XCONFD_YANGTREE_T* yt);

public:
    oam_agent_rcfd_cell_channel(XCONFD_YANGTREE_T* yt);
    virtual ~oam_agent_rcfd_cell_channel() {}
};

} //end of namespace rcfd
} //end of namespace gnb_du
#endif /* __GNB_DU_OAM_AGENT_CELL_CHANNEL__ */