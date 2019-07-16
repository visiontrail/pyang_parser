#python pyang.py -f callbackreg /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/dev/certus-5gnr-du-devicebaseinfo.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/comm -o /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/ref/reg1.cpp
#python pyang.py -f pathheader /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/dev/certus-5gnr-du-devicebaseinfo.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/comm -o /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/ref/header1.h

#python pyang.py -f callbackreg /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/cell/certus-5gnr-du-cell-cfg.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/comm -o /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/ref/reg2.cpp
#python pyang.py -f pathheader /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/cell/certus-5gnr-du-cell-cfg.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/comm -o /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/ref/header2.h

#python pyang.py -f callbackreg /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/alm/certus-5gnr-alarms.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/comm -o /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/ref/reg3.cpp
#python pyang.py -f pathheader /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/alm/certus-5gnr-alarms.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/comm -o /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang_model/gnb/ref/header3.h


#python pyang.py -f tree /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell/certus-5gnr-du-cells@2019-06-20.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/111.tree
#python pyang.py -f jsonxsl /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell/certus-5gnr-du-cells@2019-06-20.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/111.xsl
#python pyang.py -f pathheader /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell/certus-5gnr-du-cells@2019-06-20.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/111.tree


# python pyang.py -f xtypedef /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell/certus-5gnr-du-cell-base@2019-06-20.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/CertusNet/GenConfD/gnb_du_oam_agent_rcfd_cell_base.h
# python pyang.py -f xtypedef /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell/certus-5gnr-du-cell-channel@2019-06-20.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/CertusNet/GenConfD/gnb_du_oam_agent_rcfd_cell_channel.h
# python pyang.py -f xtypedef /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell/certus-5gnr-du-cell-l1@2019-06-20.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/CertusNet/GenConfD/gnb_du_oam_agent_rcfd_cell_l1.h
# python pyang.py -f xtypedef /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell/certus-5gnr-du-cell-schd@2019-06-20.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/cell -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/CertusNet/GenConfD/gnb_du_oam_agent_rcfd_cell_schd.h



# python pyang.py -f xtypedef /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/du/certus-5gnr-du-du-base-info@2019-07-08.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/du -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/CertusNet/GenConfD/gnb_du_oam_agent_rcfd_du_base_info.h
# python pyang.py -f xtypedef /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/du/certus-5gnr-du-du-log@2019-07-08.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/du -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/CertusNet/GenConfD/gnb_du_oam_agent_rcfd_du_log.h
# python pyang.py -f xtypedef /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/du/certus-5gnr-du-du-trans@2019-07-08.yang -p /home/guoliang/Desktop/WorkSpace/CertusNet/oam_du/5gran/radisys-du/src/du_app/oam_agent/yang/du -p /usr/local/confd/src/confd/yang -o ~/Desktop/WorkSpace/CertusNet/GenConfD/gnb_du_oam_agent_rcfd_du_trans.h

# CELL:
python pyang.py -f xtypedef CertusNet_Yang/GNB/cell/certus-5gnr-du-cell-base@2019-06-20.yang \
                -p CertusNet_Yang/GNB/cell \
                -p /usr/local/confd/src/confd/yang \
                -o CertusNet_Yang/GNB/output/gnb_du_oam_agent_rcfd_cell_base.h

python pyang.py -f xtypedef CertusNet_Yang/GNB/cell/certus-5gnr-du-cell-channel@2019-06-20.yang \
                -p CertusNet_Yang/GNB/cell \
                -p /usr/local/confd/src/confd/yang \
                -o CertusNet_Yang/GNB/output/gnb_du_oam_agent_rcfd_cell_channel.h

python pyang.py -f xtypedef CertusNet_Yang/GNB/cell/certus-5gnr-du-cell-l1@2019-06-20.yang \
                -p CertusNet_Yang/GNB/cell \
                -p /usr/local/confd/src/confd/yang \
                -o CertusNet_Yang/GNB/output/gnb_du_oam_agent_rcfd_cell_l1.h

python pyang.py -f xtypedef CertusNet_Yang/GNB/cell/certus-5gnr-du-cell-schd@2019-06-20.yang \
                -p CertusNet_Yang/GNB/cell \
                -p /usr/local/confd/src/confd/yang \
                -o CertusNet_Yang/GNB/output/gnb_du_oam_agent_rcfd_cell_schd.h

# DU:
python pyang.py -f xtypedef CertusNet_Yang/GNB/du/certus-5gnr-du-du-ns@2019-07-08.yang \
                -p CertusNet_Yang/GNB/du \
                -p /usr/local/confd/src/confd/yang \
                -o CertusNet_Yang/GNB/output/gnb_du_oam_agent_rcfd_du_ns.h

python pyang.py -f xtypedef CertusNet_Yang/GNB/du/certus-5gnr-du-du-base@2019-07-08.yang \
                -p CertusNet_Yang/GNB/du \
                -p /usr/local/confd/src/confd/yang \
                -o CertusNet_Yang/GNB/output/gnb_du_oam_agent_rcfd_du_base.h



