/*
 * filename: gnb_du_oam_agent_du_log.h 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#ifndef __GNB_DU_OAM_AGENT_DU_LOG__
#define __GNB_DU_OAM_AGENT_DU_LOG__

#include "gnb_du_oam_agent_rcfd_du_types.h"

namespace gnb_du 
{
namespace rcfd
{

class oam_agent_rcfd_du_log : public allocator
{
public:
    std::string file_name_; 
    std::vector<std::shared_ptr<DuModule>> du_modules_;
    std::vector<std::shared_ptr<NgpModule>> ngp_modules_;

    void read_du_modules(XCONFD_YANGTREE_T* yt);
    void read_ngp_modules(XCONFD_YANGTREE_T* yt);

public:
    oam_agent_rcfd_du_log(XCONFD_YANGTREE_T* yt);
    virtual ~oam_agent_rcfd_du_log() {}
};

} //end of namespace rcfd
} //end of namespace gnb_du
#endif /* __GNB_DU_OAM_AGENT_DU_LOG__ */