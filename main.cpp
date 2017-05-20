#include "RandomImage.h"
#include "RandomImageLoader.h"

using namespace TL;

void ExportTest(int Width = 400, int Height = 200, CharArray Testname = "Cyan/Blue/Purple")
{
    RandomImageLoader Test;

    Test.LoadRandomImage(Width,Height,GetRandomAddedInt,Testname); //Great for wavy textures
    Test.LoadRandomImage(Width,Height,GetRandomXORInt,Testname); //Linear repetition
    Test.LoadRandomImage(Width,Height,GetRandomANDInt,Testname); //Textile style
    Test.LoadRandomImage(Width,Height,GetRandomORInt,Testname); //
    Test.LoadRandomImage(Width,Height,GetRandomDividedInt,Testname); //Great for background generation
    Test.LoadRandomImage(Width,Height,GetRandomMultipliedInt,Testname); //Great for texture generation
    Test.LoadRandomImage(Width,Height,GetRandomInvertedInt,Testname); //Great for wavy textures
    Test.LoadRandomImage(Width,Height,GetRandomSubtractedInt,Testname); //Rock/ground textures

    Test.LoadRandomImage(Width,Height,RIAWonkyHopHue,Testname); //Looks like camoflage
    Test.LoadRandomImage(Width,Height,RIACyclePosHue,Testname);
    Test.LoadRandomImage(Width,Height,RIAPulsedWidthPosHue,Testname);
    Test.LoadRandomImage(Width,Height,RIACycleNegHue,Testname);
    Test.LoadRandomImage(Width,Height,RIAOddHopCyclePosHue,Testname);
    Test.LoadRandomImage(Width,Height,RIAEvenHopCyclePosHue,Testname);
    Test.LoadRandomImage(Width,Height,RIAPulsedWidthOddHopHue,Testname); //Weird desert scene
    Test.LoadRandomImage(Width,Height,RIAPulsedWidthEvenHopHue,Testname);
    Test.LoadRandomImage(Width,Height,RIAPulsedWidthToggleHue,Testname);

    while(Testname.Remove("/"));

    Test.CurrToStart();

    do
    {
        Test.SaveCurrImageToFile( Testname + CharArray(IntToString(Test.GetIter())) + CharArray(".png") );
    }while(Test.CurrNext());
}

int main(int ArgC, char *ArgV[])
{
    RandomImageLoader Test;
    Test.LoadRandomImage(3000,3000,GetRandomXORInt,"Green/Blue/Cyan/Black");
    Test.SaveCurrImageToFile(CharArray("Profoundness.png"));
    return 0;

    ExportTest(480,270,"Blue/Red/White");
    ExportTest(480,270,"Blue/Black");
    ExportTest(480,270,"Blue/Cyan");
    ExportTest(480,270,"Blue/Purple");
    ExportTest(480,270,"Blue/Pink");
    ExportTest(480,270,"Grey");
    ExportTest(480,270,"Green/Yellow/White");
    ExportTest(480,270,"Green/Black");
    ExportTest(480,270,"Green/Blue/Cyan/Black");
    ExportTest(480,270,"Green/Cyan");
    ExportTest(480,270,"Green/Yellow");
    ExportTest(480,270,"Green/Blue");
    ExportTest(480,270,"Green/Orange");
    ExportTest(480,270,"Yellow/White");
    ExportTest(480,270,"Yellow/Red/Orange");
    ExportTest(480,270,"Yellow/Orange/Green/Black");
    ExportTest(480,270,"Red/Blue/White");
    ExportTest(480,270,"Red/Yellow/White");
    ExportTest(480,270,"Red/Blue/Purple/Black");
    ExportTest(480,270,"Red/Pink/Purple");
    ExportTest(480,270,"Red/Blue/Green");
    ExportTest(480,270,"Red/Black");
    ExportTest(480,270,"Red/Blue/Purple");
    ExportTest(480,270,"Rainbow");
    ExportTest(480,270,"Cyan/White");
    ExportTest(480,270,"Cyan/Green");
    ExportTest(480,270,"Cyan/Blue/Purple");
    ExportTest(480,270,"Purple/White");
    ExportTest(480,270,"Pink/Peach");
    ExportTest(480,270,"Pink/Yellow");
    ExportTest(480,270,"Purple/Blue");
    ExportTest(480,270,"LightGreen/Yellow/Cyan");
    ExportTest(480,270,"LightGreen/Brown");
    ExportTest(480,270,"LightGreen/Yellow");

    sleep(10);

    return 0;
}
