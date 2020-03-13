python pyang.py -f moxml --moXML-version=1.0 \
                   oran_yang/CommonModels/Interfaces/o-ran-mplane-int.yang \
                -o oran_yang/All_Template/o-ran-mplane-int.xml

python pyang.py -f moxml --moXML-version=1.0 \
                   oran_yang/ImportedModels/ietf-interfaces.yang \
                -o oran_yang/All_Template/ietf-interfaces.xml

python pyang.py -f moxml --moXML-version=1.0 \
                   oran_yang/CommonModels/Operations/o-ran-operations.yang \
                -o oran_yang/All_Template/o-ran-operations.xml


cp oran_yang/All_Template/*.xml ../../modules/rru_manager/data_schema
