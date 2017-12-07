#!/bin/bash


usage() {
        echo "(script) ******************************"
        echo "(script) *   Error parameter missing    "
        echo "(script) *   Usage :                    "
        echo "(script) *   ./createInstanceGasFactory.sh <InstanceNumber>"
        echo "(script) *   <InstanceNumber>    -->  instance number allow to build the name of the xml file for this instance"
        echo "(script) *******************************"
}

echo "create an instance of the Model : start"
if [ "$1" == "" ]
then
        usage
        exit
fi


m_name=GAS_Factory_$1.xml
m_modelName=GAS_FactoryModel.xml
NOW=$(date +"%d-%m-%Y %k:%M")
sed -re "s/@Name@/$m_name/"  $m_modelName > temp.xml
sed -re "s/@DateCreation@/$NOW/"  temp.xml > $m_name
rm -rf temp.xml

echo "create an instance ($m_name) with the Model($m_modelName) : finished"
