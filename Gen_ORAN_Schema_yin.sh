
# pyang -f yin -o oran_yang/All_yin/o-ran-test.yin oran_yang/CommonModels/Operations/o-ran-test.yang \
#       -p oran_yang/ImportedModels

python pyang.py -f yin -o oran_yang/All_yin/o-ran-software-management.yin oran_yang/CommonModels/Operations/o-ran-software-management.yang \
                -p oran_yang/ImportedModels \
                -p oran_yang/CommonModels/System \
                -p oran_yang/CommonModels/Operations

python pyang.py -f yin -o oran_yang/All_yin/ietf-hardware.yin oran_yang/ImportedModels/ietf-hardware.yang \
                -p oran_yang/ImportedModels \

python pyang.py -f yin -o oran_yang/All_yin/iana-hardware.yin oran_yang/ImportedModels/iana-hardware.yang \
                -p oran_yang/ImportedModels \

python pyang.py -f yin -o oran_yang/All_yin/o-ran-hardware.yin oran_yang/CommonModels/System/o-ran-hardware.yang \
                -p oran_yang/ImportedModels \

python pyang.py -f yin -o oran_yang/All_yin/o-ran-file-management.yin oran_yang/CommonModels/Operations/o-ran-file-management.yang \
                -p oran_yang/ImportedModels \


# cp oran_yang/All_Template/*.xml ../../modules/rru_manager/data_schema
