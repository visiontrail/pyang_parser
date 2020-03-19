python pyang.py -f moxml --moXML-version=1.8 \
                   ../../yang/develop/yang/oam/flexoran-5gnr-modules.yang \
                   -p ../../yang/develop/yang/oam/fap_mgr \
                   -p ../../yang/develop/yang/oam/alarm_mgr \
                   -p ../../yang/develop/yang/oam/gnb_mgr \
                   -p ../../yang/develop/yang/oam/proc_mgr \
                   -p ../../yang/develop/yang/oam/rru_mgr \
                   -p ../../yang/develop/yang/oam/swm \
                -o ../../yang/develop/schema/flexoran-omc-oam-getschema.yin

python pyang.py -f moxml --moXML-version=1.8 \
                   ../../yang/develop/yang/du/ngran-gNBDUFunction.yang \
                   -p ../../yang/develop/yang/du \
                   -p ../../yang/develop/yang/ImportedModels \
                -o ../../yang/develop/schema/DuGetschema.yin

# python pyang.py -f moxml --moXML-version=1.0 \
#                    oran_yang/ImportedModels/ietf-interfaces.yang \
#                 -o oran_yang/All_Template/ietf-interfaces.xml

# python pyang.py -f moxml --moXML-version=1.0 \
#                    oran_yang/CommonModels/Operations/o-ran-operations.yang \
#                 -o oran_yang/All_Template/o-ran-operations.xml


# cp oran_yang/All_Template/*.xml ../../modules/rru_manager/data_schema
