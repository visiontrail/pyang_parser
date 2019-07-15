/*
 * filename: gnb_du_oam_agent_cell_base.h 
 * This header file contains implementation of OAM Agent RConfD Generate by Tools 
*/ 

#ifndef __GNB_DU_OAM_AGENT_CELL_BASE__
#define __GNB_DU_OAM_AGENT_CELL_BASE__

#include "gnb_du_oam_agent_rcfd_cell_types.h"

namespace gnb_du 
{
namespace rcfd
{

typedef struct structSNssai
{
    SstT sst; 
    SdT sd; 
} SNssai;

typedef struct structServedPlmn
{
    PlmnId plmn_id; 
    std::vector<std::shared_ptr<SNssai>> s_nssais;
} ServedPlmn;

typedef struct structNrCgi
{
    PlmnId plmn_id;
    NrCellIdT nr_cell_id; 
} NrCgi; 

typedef struct structSiPerd
{
    std::shared_ptr<SiPerdE> sib2_perd;
    std::shared_ptr<SiPerdE> sib3_perd;
} SiPerd; 

class oam_agent_rcfd_cell_base : public allocator
{
public:
    NrModeTypeE nr_mode_type_; 
    NrCgi nr_cgi_; 
    NrPciT nr_pci_; 
    Tac5GsT tac_5gs_; 
    CfgrdEpsTacT cfgrd_eps_tac_; 
    std::vector<std::shared_ptr<ServedPlmn>> served_plmns_;
    SiPerd si_perd_; 

    void read_nr_cgi(XCONFD_YANGTREE_T* yt);
    void read_served_plmns(XCONFD_YANGTREE_T* yt);
    void read_si_perd(XCONFD_YANGTREE_T* yt);
    void read_grp_s_nssai(XCONFD_YANGTREE_T* yt, SNssai& s_nssai);
    void read_grp_served_plmn(XCONFD_YANGTREE_T* yt, ServedPlmn& served_plmn);
    void read_grp_served_plmn__s_nssais(XCONFD_YANGTREE_T* yt, std::vector<std::shared_ptr<SNssai>>& s_nssais);

public:
    oam_agent_rcfd_cell_base(XCONFD_YANGTREE_T* yt);
    virtual ~oam_agent_rcfd_cell_base() {}
};

} //end of namespace rcfd
} //end of namespace gnb_du
#endif /* __GNB_DU_OAM_AGENT_CELL_BASE__ */