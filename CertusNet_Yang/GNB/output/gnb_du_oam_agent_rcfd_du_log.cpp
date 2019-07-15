/*
 * filename: gnb_du_oam_agent_rcfd_du_log.cpp 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#include "gnb_du_oam_agent_rcfd_du_log.h" 

namespace gnb_du 
{
namespace rcfd
{

oam_agent_rcfd_du_log::oam_agent_rcfd_du_log(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(file_name_, string, "file-name", yt);
    auto du_modules_yt = xconfd_yang_tree_get_node(yt, "du-modules");
    read_du_modules(du_modules_yt);
    auto ngp_modules_yt = xconfd_yang_tree_get_node(yt, "ngp-modules");
    read_ngp_modules(ngp_modules_yt);
}

void oam_agent_rcfd_du_log::read_du_modules(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(du_modules_.module_id, enum, "module-id", yt);
    xconfd_get(du_modules_.log_lvl, enum, "log-lvl", yt);
}

void oam_agent_rcfd_du_log::read_ngp_modules(XCONFD_YANGTREE_T* yt)
{
    xconfd_get(ngp_modules_.module_id, enum, "module-id", yt);
    xconfd_get(ngp_modules_.log_lvl, enum, "log-lvl", yt);
}


} // namespace rcfd 
} // namespace gnb_du