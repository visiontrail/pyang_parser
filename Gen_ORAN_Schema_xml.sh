
# pyang -f yin -o oran_yang/All_yin/o-ran-test.yin oran_yang/CommonModels/Operations/o-ran-test.yang \
#       -p oran_yang/ImportedModels

python pyang.py -f moxml --moXML-version=1.1 \
                   oran_yang/ImportedModels/ietf-interfaces.yang \
                -o oran_yang/All_Template/ietf-interfaces.xml

python pyang.py -f moxml --moXML-version=1.1 \
                   oran_yang/CommonModels/Operations/o-ran-operations.yang \
                -o oran_yang/All_Template/o-ran-operations.xml

python pyang.py -f moxml --moXML-version=1.1 \
                   oran_yang/RUSpecificModels/Operations/o-ran-uplane-conf.yang \
                -o oran_yang/All_Template/o-ran-uplane-conf.xml

python pyang.py -f moxml --moXML-version=1.1 \
                   oran_yang/CommonModels/Interfaces/o-ran-mplane-int.yang \
                -o oran_yang/All_Template/o-ran-mplane-int.xml

python pyang.py -f moxml --moXML-version=1.1 \
                   oran_yang/CommonModels/Operations/o-ran-software-management.yang \
                -o oran_yang/All_Template/o-ran-software-management.xml

python pyang.py -f moxml --moXML-version=1.1 \
                   oran_yang/CommonModels/System/o-ran-hardware.yang \
                -p oran_yang/ImportedModels \
                -o oran_yang/All_Template/o-ran-hardware.xml

python pyang.py -f moxml --moXML-version=1.1 \
                   oran_yang/ImportedModels/ietf-hardware.yang \
                -o oran_yang/All_Template/ietf-hardware.xml

python pyang.py -f moxml --moXML-version=1.1 \
                   oran_yang/CertusNet/eulowphy.yang \
                -o oran_yang/All_Template/eulowphy.xml


cp oran_yang/All_Template/*.xml ../../modules/rru_manager/data_schema
