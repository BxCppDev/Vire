#!/bin/bash


usage() {
        echo "(script) ******************************"
        echo "(script) *   Error parameter missing    "
        echo "(script) *   Usage :                    "
        echo "(script) *   ./createInstanceCALO_HVPS.sh <InstanceNumber> <Quasar_IPAdress/Port>"
        echo "(script) *   <InstanceNumber>       -->  instance number allow to build the name of the xml file for this instance"
        echo "(script) *   <Quasar_IPAdress/Port> -->  This OPCUA server ned to acess to other Quasar OPCUA server. example : 134.158.99.124:4841"
        echo "(script) *******************************"
}

echo "create an instance of the Model : start"
if [ "$1" == "" ]
then
        usage
        exit
fi


m_name=CALO_HVPS_$1.xml
m_modelName=CALO_HVPS_Model.xml
m_Quasar=$2
NOW=$(date +"%d-%m-%Y %k:%M")
sed -re "s/@Name@/$m_name/"  $m_modelName > temp.xml
sed -re "s/@QUASAR@/$m_Quasar/"  temp.xml > temp2.xml
sed -re "s/@DateCreation@/$NOW/"  temp2.xml > $m_name
rm -rf temp.xml
rm -rf temp2.xml

echo "create an instance ($m_name) with the Model($m_modelName) : finished"
