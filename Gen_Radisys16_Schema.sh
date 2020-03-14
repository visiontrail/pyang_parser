python pyang.py -f moxml --moXML-version=1.0 \
                   ../develop/yang/du/cell/certus-5gnr-du-cells@2019-06-20.yang \
                   -p ../develop/yang/ImportedModels \
                   -p ../develop/yang/du/cell \
                   -p ../develop/yang/du/du \
                -o ../develop/schema_data/flexoran-omc-du-cell-getschema.yin

python pyang.py -f moxml --moXML-version=1.0 \
                   ../develop/yang/du/du/certus-5gnr-du-du@2019-07-08.yang \
                   -p ../develop/yang/ImportedModels \
                   -p ../develop/yang/du/cell \
                   -p ../develop/yang/du/du \
                -o ../develop/schema_data/flexoran-omc-du-du-getschema.yin

# python pyang.py -f moxml --moXML-version=1.0 \
#                    oran_yang/ImportedModels/ietf-interfaces.yang \
#                 -o oran_yang/All_Template/ietf-interfaces.xml

# python pyang.py -f moxml --moXML-version=1.0 \
#                    oran_yang/CommonModels/Operations/o-ran-operations.yang \
#                 -o oran_yang/All_Template/o-ran-operations.xml


# cp oran_yang/All_Template/*.xml ../../modules/rru_manager/data_schema
