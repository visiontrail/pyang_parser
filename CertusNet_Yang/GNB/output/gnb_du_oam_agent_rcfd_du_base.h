/*
 * filename: gnb_du_oam_agent_du_base.h 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#ifndef __GNB_DU_OAM_AGENT_DU_BASE__
#define __GNB_DU_OAM_AGENT_DU_BASE__

#include "gnb_du_oam_agent_rcfd_du_types.h"

namespace gnb_du 
{
namespace rcfd
{

typedef struct structDrx
{
    uint32_t inactivity_tmr; 
    uint32_t retx_tmr_dl; 
    uint32_t retx_tmr_ul; 
    uint32_t long_cycle; 
    uint32_t short_cycle; 
    uint32_t short_cycle_tmr; 
} Drx;

typedef struct structSctp
{
    IpAddr dst; 
    IpAddr src; 
    CfgParams cfg_params; 
} Sctp;

typedef struct structLog
{
    std::string file_name; 
    std::vector<std::shared_ptr<DuModule>> du_modules;
    std::vector<std::shared_ptr<NgpModule>> ngp_modules;
} Log;

typedef struct structDst
{
    IpAddr ip_addr;
    uint32_t port; 
} Dst; 

typedef struct structSrc
{
    IpAddr ip_addr;
    uint32_t port; 
} Src; 

typedef struct structCfgParams
{
    uint16_t num_outbound_streams; 
    uint16_t max_inbound_streams; 
    uint16_t max_init_attempts; 
    uint16_t hb_interval; 
    uint16_t max_path_retx; 
} CfgParams; 

typedef struct structUe
{
    uint16_t max_ue_supported; 
    uint16_t rnti_start; 
    uint16_t max_num_rnti; 
} Ue; 

typedef struct structEgtpu
{
    IpAddr ip_addr;
    uint32_t teid_min; 
    uint32_t teid_max; 
} Egtpu; 

typedef struct structF1UFlowCtrl
{
    uint16_t max_rlc_sdu_q_size; 
    uint16_t rlc_sdu_q_lwr_thr; 
    uint16_t rlc_sdu_q_upr_thr; 
    uint16_t nrup_flw_ctrl_tmr; 
    bool en_nrup_missing_rept; 
    uint32_t read_ingress_pkts_per_tti; 
} F1UFlowCtrl; 

typedef struct structDumodules
{
    DuModuleIdE module_id; 
    LogLvlE log_lvl; 
} DuModules; 

typedef struct structNgpmodules
{
    NgpModuleIdE module_id; 
    LogLvlE log_lvl; 
} NgpModules; 

class oam_agent_rcfd_du_base : public allocator
{
public:
    uint64_t gnb_du_id_; 
    std::string gnb_du_name_; 
    uint8_t max_cell_supported_; 
    Ue ue_; 
    Sctp sctp_; 
    Egtpu egtpu_; 
    F1UFlowCtrl f1u_flow_ctrl_; 
    std::shared_ptr<Drx> drx_;
    Log log_; 

    void read_ue(XCONFD_YANGTREE_T* yt);
    void read_sctp(XCONFD_YANGTREE_T* yt);
    void read_egtpu(XCONFD_YANGTREE_T* yt);
    void read_f1u_flow_ctrl(XCONFD_YANGTREE_T* yt);
    void read_drx(XCONFD_YANGTREE_T* yt);
    void read_log(XCONFD_YANGTREE_T* yt);
    void read_grp_drx(XCONFD_YANGTREE_T* yt, Drx& drx);
    void read_grp_sctp(XCONFD_YANGTREE_T* yt, Sctp& sctp);
    void read_grp_sctp__dst(XCONFD_YANGTREE_T* yt, IpAddr& dst);
    void read_grp_sctp__src(XCONFD_YANGTREE_T* yt, IpAddr& src);
    void read_grp_sctp__cfg_params(XCONFD_YANGTREE_T* yt, CfgParams& cfg_params);
    void read_grp_log(XCONFD_YANGTREE_T* yt, Log& log);
    void read_grp_log__du_modules(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<DuModules>>& du_modules);
    void read_grp_log__ngp_modules(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<NgpModules>>& ngp_modules);

public:
    oam_agent_rcfd_du_base(XCONFD_YANGTREE_T* yt);
    virtual ~oam_agent_rcfd_du_base() {}
};

} //end of namespace rcfd
} //end of namespace gnb_du
#endif /* __GNB_DU_OAM_AGENT_DU_BASE__ */