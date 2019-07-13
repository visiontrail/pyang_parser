/*!********************************************************************************************************************
 * file        gnb_du_oam_agent_rcfd_cell_base.h
 *
 * brief       This header file contains implementation of OAM Agent RConfD Cell Base
 *********************************************************************************************************************/

#ifndef __GNB_DU_OAM_AGENT_RCFD_CELL_BASE_H__
#define __GNB_DU_OAM_AGENT_RCFD_CELL_BASE_H__

#include "gnb_du_oam_agent_rcfd_cell_types.h"


namespace gnb_du
{
namespace rcfd
{

typedef struct structNrCgi
{
   PlmnId plmn_id;
   NrCellIdT nr_cell_id;
} NrCgi;

typedef struct structSNssai
{
   SstT sst;
   SdT sd;
} SNssai;

typedef std::shared_ptr<SNssai> SNssaiPtr;
typedef std::vector<SNssaiPtr> SNssaiPtrVector;

typedef struct structServedPlmn
{
   PlmnId plmn_id;
   SNssaiPtrVector s_nssais;
} ServedPlmn;

typedef std::shared_ptr<ServedPlmn> ServedPlmnPtr;
typedef std::vector<ServedPlmnPtr> ServedPlmnPtrVector;

typedef struct structSiPerd
{
   std::shared_ptr<SiPerdE> sib2_perd;
   std::shared_ptr<SiPerdE> sib3_perd;
} SiPerd;

class oam_agent_rcfd_cell_base : public allocator
{
public:
   NrCgi nr_cgi_;
   NrPciT nr_pci_;
   NrModeTypeE nr_mode_type_;
   Tac5GsT tac_5gs_;
   CfgrdEpsTacT cfgrd_eps_tac_;
   ServedPlmnPtrVector served_plmns_;
   SiPerd si_perd_;

private:
   void read_nr_cgi(XCONFD_YANGTREE_T* yt);
   void read_served_plmns(XCONFD_YANGTREE_T* yt);
   void read_served_plmns__s_nssais(XCONFD_YANGTREE_T* yt, SNssaiPtrVector& s_nssais);
   void read_grp_s_nssai(XCONFD_YANGTREE_T* yt, SNssai& s_nssais);
   void read_si_perd(XCONFD_YANGTREE_T* yt);

public:
   oam_agent_rcfd_cell_base(XCONFD_YANGTREE_T* yt);
   virtual ~oam_agent_rcfd_cell_base() {}
};

} // namespace rcfd
} // namespace gnb_du

#endif /*__GNB_DU_OAM_AGENT_RCFD_CELL_BASE_H__*/

