#include "ImageProcessTest.h"
#include "Filesystem.h"

int main(int argc,char *argv[])
{
	ImageProcessTest imageProcessTest;
    imageProcessTest.TestLBP();

    vector<string> fileNameList;
    QQ::GetFileNameList("D:/Image/Color/","*.jpg",fileNameList,true,true);
    for(int i=0;i<fileNameList.size();++i)
    {
        printf("%s\n",fileNameList[i].c_str());
    }
	
	return 0;
}
