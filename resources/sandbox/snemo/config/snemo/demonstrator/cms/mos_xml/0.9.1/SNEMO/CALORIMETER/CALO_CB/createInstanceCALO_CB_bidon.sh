#!/bin/bash


usage() {
        echo "(script) ******************************"
        echo "(script) *   Error parameter missing    "
        echo "(script) *   Usage :                    "
        echo "(script) *   ./createInstanceCALO_CB.sh <InstanceNumber>"
        echo "(script) *   <InstanceNumber>    -->  instance number allow to build the name of the xml file for this instance"
        echo "(script) *******************************"
}

echo "create an instance of the Model : start"
if [ "$1" == "" ]
then
        usage
        exit
fi
if [ "$2" == "" ]
then
        usage
        exit
fi
if [ "$3" == "" ]
then
        usage
        exit
fi


m_name=CALO_CB_$3.xml
m_modelName=ControlBoardModel_Bidon.xml
NOW=$(date +"%d-%m-%Y %k:%M")
sed -re "s/@Address@/$1/" $m_modelName > temp.xml
sed -re "s/@Name@/$m_name/"  temp.xml > temp2.xml
sed -re "s/@DateCreation@/$NOW/"  temp2.xml > temp3.xml
sed -re "s/@Port@/$2/"  temp3.xml > $m_name
rm -rf temp.xml
rm -rf temp2.xml
rm -rf temp3.xml

echo "create an instance ($m_name) with the Model($m_modelName) : finished"
