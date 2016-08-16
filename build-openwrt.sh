#
# script for building openwrt image
#
#!/bin/sh

LOCAL_PATH=`pwd`
WORK_PATH=$LOCAL_PATH/build_dir
OPENWRT_PACKAGE=$LOCAL_PATH/openWRT

#create work folder
mkdir -p $WORK_PATH

if [ $? -ne 0 ]; then 
	echo "Can't create folder: $WORK_PATH, Please check  if you have sudo access."
	exit
fi

#goto work folder
echo "goto $WORK_PATH...."
cd $WORK_PATH

#get openwrt source code
echo "======================= download openwrt objects ==================="

CODE_URL=svn://svn.openwrt.org.cn/openwrt/trunk 
TRUNK=46386
LOCAL_TRUNK=openwrt-$TRUNK-code

svn co $CODE_URL -r $TRUNK $LOCAL_TRUNK

if [ $? -ne 0 ]; then
	echo "failed to download openwrt objects..., please check." 
	exit
fi

#update openwrt packages
echo "======================= update openwrt pakcages ====================="
cd ./$LOCAL_TRUNK

./scripts/feeds update -a
./scripts/feeds install -a

cd -

#put our owns packages into openwrt package
echo "put local package into openwrt...."
cp -rf $OPENWRT_PACKAGE/* ./$LOCAL_TRUNK

#IoTGateway source
V2_SOURCE=$LOCAL_PATH/V2
V2_DIST_DIR=$WORK_PATH/$LOCAL_TRUNK/package/network/IoT/gateway/files

cp -rf $V2_SOURCE/* $V2_DIST_DIR

#make openwrt image 
echo "======================= building openwrt image ====================="

cd ./$LOCAL_TRUNK

make V=99

