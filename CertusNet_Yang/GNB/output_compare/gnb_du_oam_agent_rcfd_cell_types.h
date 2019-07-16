/*!********************************************************************************************************************
 * file        gnb_du_oam_agent_rcfd_cell.h
 *
 * brief       This header file contains implementation of OAM Agent RConfD Cell
 *********************************************************************************************************************/

#ifndef __GNB_DU_OAM_AGENT_RCFD_CELL_TYPES_H__
#define __GNB_DU_OAM_AGENT_RCFD_CELL_TYPES_H__

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "xconfd_api.h"
#include "gnb_du_common.h"
#include "gnb_du_oam_comm.h"


namespace gnb_du
{
namespace rcfd
{

typedef enum enumT310E
{
   T310_MS0,
   T310_MS50,
   T310_MS100,
   T310_MS200,
   T310_MS500,
   T310_MS1000,
   T310_MS2000
} T310E;

typedef enum enumRestrSetE
{
   RESTR_UNRESTRICTED_SET,
   RESTR_TYPE_A,
   RESTR_TYPE_B
} RestrSetE;

typedef enum enumSsbPerRachOccE
{
   SSB_ONE_EIGHT,
   SSB_ONE_FOURTH,
   SSB_ONE_HALF,
   SSB_ONE,
   SSB_TWO,
   SSB_FOUR,
   SSB_EIGHT,
   SSB_SIXTEEN
} SsbPerRachOccE;



typedef enum enumBwpInactiveTimerE
{
   BWP_MS2,
   BWP_MS3,
   BWP_MS4,
   BWP_MS5,
   BWP_MS6,
   BWP_MS8,
   BWP_MS10,
   BWP_MS20,
   BWP_MS30,
   BWP_MS40,
   BWP_MS50,
   BWP_MS60,
   BWP_MS80,
   BWP_MS100,
   BWP_MS200,
   BWP_MS300,
   BWP_MS500,
   BWP_MS750,
   BWP_MS1280,
   BWP_MS1920,
   BWP_MS2560,
   BWP_SPARE10,
   BWP_SPARE9,
   BWP_SPARE8,
   BWP_SPARE7,
   BWP_SPARE6,
   BWP_SPARE5,
   BWP_SPARE4,
   BWP_SPARE3,
   BWP_SPARE2,
   BWP_SPARE1
} BwpInactiveTimerE;

typedef int8_t QQualMinT; // range "-43..-12";


typedef enum enumRaRspWinE
{
   WIN_SL1,
   WIN_SL2,
   WIN_SL4,
   WIN_SL8,
   WIN_SL10,
   WIN_SL20,
   WIN_SL40,
   WIN_SL80
} RaRspWinE;

typedef RegBndlSizeE InterlvrSizeE;





typedef enum enumMsgPowerOffsetGrpBE
{
   OFFSET_MINU_INFINITY,
   OFFSET_DB0,
   OFFSET_DB5,
   OFFSET_DB8,
   OFFSET_DB10,
   OFFSET_DB12,
   OFFSET_DB15,
   OFFSET_DB18
} MsgPowerOffsetGrpBE;




typedef enum enumHarqAckCbTypeE
{
   CB_SS,
   CB_DYN
} HarqAckCbTypeE;


typedef enum enumPrachRootSeqIdxTypeE
{
   SEQ_L839,
   SEQ_L139
} PrachRootSeqIdxTypeE;

typedef uint16_t OffsetPointACarrierT; // range "0..2199"



typedef uint16_t CfgrdEpsTacT; // Configured EPS TAC

typedef enum enumScsCmnE
{
   SCS15OR60,
   SCS30OR120
} ScsCmnE;

typedef uint8_t SymsNumT; // range "0..13"



typedef enum enumPerdBsrTimerE
{
   BSR_SF1,
   BSR_SF5,
   BSR_SF10,
   BSR_SF16,
   BSR_SF20,
   BSR_SF32,
   BSR_SF40,
   BSR_SF64,
   BSR_SF80,
   BSR_SF128,
   BSR_SF160,
   BSR_SF320,
   BSR_SF640,
   BSR_SF1280,
   BSR_SF2560,
   BSR_INFINITY
} PerdBsrTimerE;

typedef uint16_t NrPciT; // range "0..1007"; Physical Cell ID

typedef uint8_t CoresetIdT; // range "0..11"


typedef enum enumOperStateE
{
   DEACTIVATING,
   UNACTIVATED,
   ACTIVATING,
   ACTIVATED
} OperStateE;

typedef uint8_t SsZeroT; // range "0..15"

typedef enum enumRetxBsrTimerE
{
   RETX_BSR_SF10,
   RETX_BSR_SF20,
   RETX_BSR_SF40,
   RETX_BSR_SF80,
   RETX_BSR_SF160,
   RETX_BSR_SF320,
   RETX_BSR_SF640,
   RETX_BSR_SF1280,
   RETX_BSR_SF2560,
   RETX_BSR_SF5120,
   RETX_BSR_SF10240,
   RETX_BSR_SPARE5,
   RETX_BSR_SPARE4,
   RETX_BSR_SPARE3,
   RETX_BSR_SPARE2,
   RETX_BSR_SPARE1
} RetxBsrTimerE;

typedef enum enumN310E
{
   N310_N1,
   N310_N2,
   N310_N3,
   N310_N4,
   N310_N6,
   N310_N8,
   N310_N10,
   N310_N20
} N310E;

typedef enum enumDciFormatE
{
   FORMATS0_0_AND_1_0,
   FORMATS0_1_And_1_1
} DciFormatE;



typedef enum enumSiPerdE
{
   SI_RF8,
   SI_RF16,
   SI_RF32,
   SI_RF64,
   SI_RF128,
   SI_RF256,
   SI_RF512
} SiPerdE;
typedef enum enumPowerRampStepE
{
   STEP_DB0,
   STEP_DB2,
   STEP_DB4,
   STEP_DB6
} PowerRampStepE;

typedef enum enumPucchGrpHoppingE
{
   HOPPING_NEITHER,
   HOPPING_ENABLE,
   HOPPING_DISABLE
} PucchGrpHoppingE;

typedef enum enumAminStateE
{
   BLOCKED,
   UNBLOCKED
} AdminStateE;

typedef enum enumNsE
{
   NS_FOUR,
   NS_TWO,
   NS_ONE
} NsE;

typedef enum enumT301E
{
   T301_MS100,
   T301_MS200,
   T301_MS300,
   T301_MS400,
   T301_MS600,
   T301_MS1000,
   T301_MS1500,
   T301_MS2000
} T301E;

typedef enum enumSiWinLenE
{
   SI_SL5,
   SI_SL10,
   SI_SL20,
   SI_SL40,
   SI_SL80,
   SI_SL160,
   SI_SL320,
   SI_SL640,
   SI_SL1280
} SiWinLenE;


typedef uint8_t BwpIdT; // range "1..4"

typedef uint32_t ArfcnT; // range "0..3279165"

typedef int8_t QRxLvlMinT; // range "-70..-22";

typedef enum enumT319E
{
   T319_MS100,
   T319_MS200,
   T319_MS300,
   T319_MS400,
   T319_MS600,
   T319_MS1000,
   T319_MS1500,
   T319_MS2000
} T319E;

typedef enum enumPreambleTransMaxE
{
   TRANS_N3,
   TRANS_N4,
   TRANS_N5,
   TRANS_N6,
   TRANS_N7,
   TRANS_N8,
   TRANS_N10,
   TRANS_N20,
   TRANS_N50,
   TRANS_N100,
   TRANS_N200
} PreambleTransMaxE;

typedef uint8_t SsIdT; // range "0..39"

typedef enum enumMsg1FdmE
{
   FDM_F1,
   FDM_F2,
   FDM_F4,
   FDM_F8
} Msg1FdmE;

typedef enum enumXOverheadE
{
   XOH6,
   XOH12,
   XOH18
} XOverheadE;

typedef enum enumT311E
{
   T311_MS1000,
   T311_MS3000,
   T311_MS5000,
   T311_MS10000,
   T311_MS15000,
   T311_MS20000,
   T311_MS30000
} T311E;

typedef uint32_t Tac5GsT; // range "0..16777215"; BIT STRING (24); 5GS Tracking Area Code

typedef enum enumRaContResTmrE
{
   CONT_SF8,
   CONT_SF16,
   CONT_SF24,
   CONT_SF32,
   CONT_SF40,
   CONT_SF48,
   CONT_SF56,
   CONT_SF64
} RaContResTmrE;

typedef enum enumPagingCycleE
{
   PAGING_RF32,
   PAGING_RF64,
   PAGING_RF128,
   PAGING_RF256
} PagingCycleE;

typedef enum enumT300E
{
   T300_MS100,
   T300_MS200,
   T300_MS300,
   T300_MS400,
   T300_MS600,
   T300_MS1000,
   T300_MS1500,
   T300_MS2000
} T300E;

typedef enum enumScsE
{
   SCS_KHZ15,
   SCS_KHZ30,
   SCS_KHZ60,
   SCS_KHZ120,
   SCS_KHZ240,
   SCS_KHZ320,
   SCS_SPARE2,
   SCS_SPARE1
} ScsE;

typedef enum enumSsbScellPerdE
{
   SSP_MS5,
   SSP_MS10,
   SSP_MS20,
   SSP_MS40,
   SSP_MS80,
   SSP_MS160
} SsbScellPerdE;

typedef uint16_t SlotsNumT; // range "0..320"

typedef enum enumN311E
{
   N311_N1,
   N311_N2,
   N311_N3,
   N311_N4,
   N311_N5,
   N311_N6,
   N311_N8,
   N311_N10
} N311E;

typedef enum enumPagingFrameOffsetTypeE
{
   ONE_T,
   HALF_T,
   QUARTER_T,
   ONE_EIGHTH_T,
   ONE_SIXTEENTH_T
} PagingFrameOffsetTypeE;

typedef enum enumModiPerdCoeffE
{
   COEFF_N2,
   COEFF_N4,
   COEFF_N8,
   COEFF_N16
} ModiPerdCoeffE;

typedef enum enumAlignmentTimerE
{
   ALIGN_MS500,
   ALIGN_MS750,
   ALIGN_MS1280,
   ALIGN_MS1920,
   ALIGN_MS2560,
   ALIGN_MS5120,
   ALIGN_MS10240,
   ALIGN_INFINITY
} AlignmentTimerE;


typedef enum enumDmrsTypeAPosE
{
   DMRS_POS2,
   DMRS_POS3
} DmrsTypeAPosE;

typedef enum enumCandidatesNumE
{
   CANDIDATE_N0,
   CANDIDATE_N1,
   CANDIDATE_N2,
   CANDIDATE_N3,
   CANDIDATE_N4,
   CANDIDATE_N5,
   CANDIDATE_N6,
   CANDIDATE_N8
} CandidatesNumE;
typedef uint8_t SsbScOffsetT; // range "0..15"

typedef enum enumTransPerdE
{
   TRANS_MS0P5,
   TRANS_MS0P625,
   TRANS_MS1,
   TRANS_MS1P25,
   TRANS_MS2,
   TRANS_MS2P5,
   TRANS_MS5,
   TRANS_MS10
} TransPerdE;

typedef enum enumChannelBwE
{
   BW_MHZ5,
   BW_MHZ10,
   BW_MHZ15,
   BW_MHZ20,
   BW_MHZ25,
   BW_MHZ30,
   BW_MHZ40,
   BW_MHZ50,
   BW_MHZ60,
   BW_MHZ80,
   BW_MHZ100,
   BW_MHZ200,
   BW_MHZ400
} ChannelBwE;

typedef enum enumRegBndlSizeE
{
   BNDL_N2,
   BNDL_N3,
   BNDL_N6
} RegBndlSizeE;

typedef enum enumMonSlotPerdOffsetTypeE
{
   SPO_SL1,
   SPO_SL2,
   SPO_SL4,
   SPO_SL5,
   SPO_SL8,
   SPO_SL10,
   SPO_SL16,
   SPO_SL20,
   SPO_SL40,
   SPO_SL80,
   SPO_SL160,
   SPO_SL320,
   SPO_SL640,
   SPO_SL1280,
   SPO_SL2560
} MonSlotPerdOffsetTypeE;
typedef enum enumNrModeTypeE
{
   NR_FDD,
   NR_TDD
} NrModeTypeE;

typedef uint64_t NrCellIdT; // BIT STRING (36)

typedef uint8_t SstT; // BIT STRING (8); Slice/Service Type


typedef enum enumPrecoderGranularityE
{
   SAME_AS_REG_BUNDLE,
   ALL_CONTIGUOUS_RBS
} PrecoderGranularityE;

typedef enum enumPdschHarqProcessesNumE
{
   HARQ_PROC_N2,
   HARQ_PROC_N4,
   HARQ_PROC_N6,
   HARQ_PROC_N10,
   HARQ_PROC_N12,
   HARQ_PROC_N16
} PdschHarqProcessesNumE;


typedef uint8_t TciStateIdT; // range "0..127"

typedef uint8_t CoresetZeroT; // range "0..15"

typedef enum enumMaxCbgsPerTbE
{
   CBGS_N2,
   CBGS_N4,
   CBGS_N6,
   CBGS_N8
} MaxCbgsPerTbE;

typedef enum enumRaMsg3SizeGrpAE
{
   RA_B56,
   RA_B144,
   RA_B208,
   RA_B256,
   RA_B282,
   RA_B480,
   RA_B640,
   RA_B800,
   RA_B1000,
   RA_B72,
   RA_SPARE6,
   RA_SPARE5,
   RA_SPARE4,
   RA_SPARE3,
   RA_SPARE2,
   RA_SPARE1
} RaMsg3SizeGrpAE;


typedef uint32_t SdT; // BIT STRING (32); Slice Differentiator

typedef enum enumTransPerdV1530E
{
   TRANS_MS3_V1530,
   TRANS_MS4_V1530
} TransPerdV1530E;





typedef struct structPlmnId
{
   std::string mcc; // length 3; pattern "[0-9]*";
   std::string mnc; // length "2..3"; pattern "[0-9]*";
} PlmnId;

typedef std::shared_ptr<PlmnId> PlmnIdPtr;
typedef std::vector<PlmnIdPtr> PlmnIdPtrVector;

void read_grp_plmn_id(XCONFD_YANGTREE_T* yt, PlmnId& plmn_id)
{
   xconfd_get(plmn_id.mcc, string, "mcc", yt);
   xconfd_get(plmn_id.mnc, string, "mnc", yt);
}

} // namespace rcfd
} // namespace gnb_du

#endif /*__GNB_DU_OAM_AGENT_RCFD_CELL_TYPES_H__*/

