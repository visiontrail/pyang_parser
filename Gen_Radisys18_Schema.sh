python pyang.py -f moxml --moXML-version=RadisysDU_1.8_version1.0.3 \
                   ../../yang/develop/yang/oam/flexoran-5gnr-modules.yang \
                   -p ../../yang/develop/yang/oam/fap_mgr \
                   -p ../../yang/develop/yang/oam/alarm_mgr \
                   -p ../../yang/develop/yang/oam/gnb_mgr \
                   -p ../../yang/develop/yang/oam/proc_mgr \
                   -p ../../yang/develop/yang/oam/rru_mgr \
                   -p ../../yang/develop/yang/oam/swm \
                -o ../../yang/develop/schema/flexoran-omc-oam-getschema.yin

python pyang.py -f moxml --moXML-version=RadisysDU_1.8_version1.0.3 \
                   ../../yang/develop/yang/du/ngran-gNBDUFunction.yang \
                   -p ../../yang/develop/yang/du \
                   -p ../../yang/develop/yang/ImportedModels \
                -o ../../yang/develop/schema/DuGetschema.yin

# cp oran_yang/All_Template/*.xml ../../modules/rru_manager/data_schema
