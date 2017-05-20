#ifndef G_ANTIKEYLOGGER_HPP
#define G_ANTIKEYLOGGER_HPP

/*
Generated on: 2016-06-11-15.16.14

Version: 1.0
*/

/*
Notes:


*/

/*
Suggestions for improvements:


*/

#include <SFML/Graphics.hpp>
#include "ErrorMessage.h"
#include "TextInputBox.hpp"
#include "Random.h"
#include "CommonFunctions.h"
#include "TemplateList.h"

#ifndef AKL_FIRST_MESSAGE
#define AKL_FIRST_MESSAGE "Please type "
#endif

#ifndef AKL_SECOND_MESSAGE
#define AKL_SECOND_MESSAGE " garbage characters"
#endif

#ifndef AKL_FIRST_NORMAL_MESSAGE
#define AKL_FIRST_NORMAL_MESSAGE "Please enter the next "
#endif

#ifndef AKL_SECOND_NORMAL_MESSGE
#define AKL_SECOND_NORMAL_MESSGE " characters of your password"
#endif

#ifndef AKL_NORMAL_MESSAGE
#define AKL_NORMAL_MESSAGE "Please type the next 1 character of your password!"
#endif


namespace TL
{

//ERRORCLASSFUNCTION(AntiKeylogger,,,RETURN_BOOL)

class AntiKeylogger : public sf::Drawable
{
    private:
    protected:

        Button Message;
        TextInputBox TextInput;
        bool Offset;
        bool GarbageMode;
        uint8_t GarbageInput;
        TemplateList<uint8_t> NumberOfGarbageCharacters;

        //3,1,4,1,10,1
        //1,3,1,4,1,10

        TemplateList<SIZE_TYPE> CalculateNonGarbage() const
        {
            ERRORCLASSFUNCTION(AntiKeylogger,CalculateNonGarbage,!NumberOfGarbageCharacters.IsValid(),return TemplateList<SIZE_TYPE>();)

            TemplateList<SIZE_TYPE> Temp;
            TemplateIterator<uint8_t> Temp2(NumberOfGarbageCharacters);

            Temp2.CurrToStart();

            SIZE_TYPE Iter = 0;

            if(!Offset)
            {
                Temp.Append(0);
                Iter++;
            }

            do
            {
                Iter += Temp2.GetCurrItem();
                Temp.Append(Iter);
                Iter++;//We move by one for the normal character
            }while(Temp2.CurrNext());

            return Temp;
        }

        void BackspaceReset()
        {
            NumberOfGarbageCharacters = TemplateList<uint8_t>();
            Offset = GetRandomBool();
            GarbageInput = 0;
            if(Offset)
            {
                ConstructGarbageNumber();
            }
            else
            {
                ConstructNormalNumber();
            }
            TextInput.BackspaceReset();
        }

        const bool ConstructGarbageNumber()
        {
            uint8_t Mod;

            do{
                Mod = GetRandomXORInt(GetRandomAddedInt(GetRandomANDInt(GetRandomInt())));
                Mod = (Mod%10);
            }while(Mod < 1);


            ERRORCLASSFUNCTION(AntiKeylogger,ConstructGarbageNumber,!NumberOfGarbageCharacters.Append(Mod),RETURN_BOOL)

            GarbageMode = true;

            sf::String GarbageMessage(AKL_FIRST_MESSAGE);
            GarbageMessage += IntToString(NumberOfGarbageCharacters.GetEndItem());
            GarbageMessage += AKL_SECOND_MESSAGE;

            Message.SetText(GarbageMessage);
            return true;
        }

        void UpdateGarbageNumber()
        {
            sf::String GarbageMessage(AKL_FIRST_MESSAGE);
            GarbageMessage += IntToString(NumberOfGarbageCharacters.GetEndItem() - GarbageInput);
            GarbageMessage += AKL_SECOND_MESSAGE;

            Message.SetText(GarbageMessage);
        }

        void ConstructNormalNumber()
        {
            GarbageInput = 0;
            GarbageMode = false;
            Message.SetText(AKL_NORMAL_MESSAGE);
        }

        void Clear()
        {
            Offset = false;
            GarbageMode = false;
            GarbageInput = 0;
        }

    public:

        void Open(){Clear(); TextInput.Open(); Message.Open(); NumberOfGarbageCharacters.Open();}
        void Close(){TextInput.Close(); Message.Close(); NumberOfGarbageCharacters.Close(); Clear();}
        void Reset(){Close(); Open();}

        const bool IsEmpty() const { return Message.IsEmpty() && TextInput.IsEmpty() && NumberOfGarbageCharacters.IsEmpty() && Offset == false && GarbageMode == false; }

        AntiKeylogger(){Open();}
        ~AntiKeylogger(){Close();}

        inline const Button &GetMessage() const { return Message; }
        inline const TextInputBox &GetTextInput() const { return TextInput; }

        inline void SetTextInput(const TextInputBox &ItemCopy){ TextInput = ItemCopy; }
        inline void SetMessage(const Button &ItemCopy){ Message = ItemCopy; }

        const bool SetAntiKeylogger(const AntiKeylogger &ItemCopy)
        {
            if(ItemCopy.IsEmpty())
            {
                Close();
                return true;
            }
            Message.SetButton(ItemCopy.Message);
            TextInput.SetTextInputBox(ItemCopy.TextInput);

            Offset = ItemCopy.Offset;
            GarbageMode = ItemCopy.GarbageMode;
            GarbageInput = ItemCopy.GarbageInput;
            NumberOfGarbageCharacters = ItemCopy.NumberOfGarbageCharacters;

            return true;
        }

        const bool PreloadConfig(const CharArray &Font, const sf::String &HelpTextCopy = "Please input text", const TIB_Mode &ModeCopy = TIB_OneChar, const SIZE_TYPE &NumberOfCharsCopy = 0, const sf::Uint32 &OneCharCopy = 42, const sf::Color NormalTextColourCopy = sf::Color(0,0,0), const sf::Color HelpTextColourCopy = sf::Color(200,200,200))
        {
            ERRORCLASSFUNCTION(AntiKeylogger,PreloadConfig,!TextInput.PreloadConfig(Font,HelpTextCopy,ModeCopy,NumberOfCharsCopy,OneCharCopy,NormalTextColourCopy,HelpTextColourCopy),RETURN_BOOL)
            ERRORCLASSFUNCTION(AntiKeylogger,PreloadConfig,!Message.SetFont(Font),RETURN_BOOL)

            Offset = GetRandomBool();
            Offset = true;

            Message.SetButton(" ",sf::Color(128,128,128,0),sf::Color(20,20,20,0),0.0);

            if(Offset)
            {
                ERRORCLASSFUNCTION(AntiKeylogger,PreloadConfig,!ConstructGarbageNumber(),RETURN_BOOL)
            }
            else
            {
                ConstructNormalNumber();
            }
            SetPosition(0,0);

            return true;
        }

        inline const sf::String GetGarbagedMemory() const
        {
            return TextInput.GetDecryptedMemory();
        }

        inline const sf::String GetNonGarbagedMemory() const
        {

            if(NumberOfGarbageCharacters.IsEmpty())
            {
                return sf::String();
            }

            sf::String Temp = TextInput.GetDecryptedMemory();
            TemplateList<SIZE_TYPE> Temp2 = CalculateNonGarbage();

            ERRORCLASSFUNCTION(AntiKeylogger,GetNonGarbagedMemory,!Temp2.IsValid(),return sf::String();)

            sf::String Temp3;

            Temp2.CurrToStart();

            do
            {
                if(Temp2.GetCurrItem() < Temp.getSize())
                {
                    Temp3 += Temp[Temp2.GetCurrItem()];
                }
                else
                {
                    break;
                }
            }while(Temp2.CurrNext());

            return Temp3;
        }

        const bool IsMouseWithinCoords(sf::RenderWindow &RenderWindow, const bool ToggleHelp)
        {
            return TextInput.IsMouseWithinCoords(RenderWindow,ToggleHelp);
        }

        const uint8_t IsTextEntered(sf::Event &Event)
        {
            if(TextInput.GetCanInput())
            {
                if(Event.type == sf::Event::TextEntered)
                {
                    /*
                    Backspacing incomplete garbage characters is not possible,
                    it would really mess up counting and poses a possible security vulnerability
                    so we have to wipe the entire password
                    */
                    if(Event.text.unicode == TIB_BACKSPACE)
                    {
                        BackspaceReset();
                        return 0;
                    }
                    else
                    {
                        //Check the user input information
                        uint8_t ITE = TextInput.IsTextEntered(Event);

                        if(ITE == 0)
                        {
                            if(GarbageMode)
                            {
                                //If garbage we count until it matches the number of garbage characters required
                                ++GarbageInput;

                                if(GarbageInput == NumberOfGarbageCharacters.GetEndItem())
                                {
                                    ConstructNormalNumber();
                                }
                                else
                                {
                                    UpdateGarbageNumber();
                                }
                            }
                            else
                            {
                                ConstructGarbageNumber();
                            }
                        }
                    }
                    return 0;
                }
            }
            return 1;

        }

        const sf::Vector2f &GetPosition() { return Message.GetPosition(); }

        void SetPosition(const sf::Vector2f &Position)
        {
            Message.SetPosition(Position);
            sf::FloatRect Temp = Message.GetLocalBounds();
            TextInput.SetPosition(Position.x+((Temp.width/2)-(TextInput.GetLocalBounds().width/2)),Position.y+Temp.height);
        }

        void SetPosition(const float x, const float y)
        {
            Message.SetPosition(x,y);
            sf::FloatRect Temp = Message.GetLocalBounds();
            TextInput.SetPosition(x+((Temp.width/2)-(TextInput.GetLocalBounds().width/2)),y+Temp.height);

        }

        virtual void draw(sf::RenderTarget& RenderTarget, sf::RenderStates states) const
        {
            RenderTarget.draw(Message);
            RenderTarget.draw(TextInput);
        }

        void DrawToWindow(sf::RenderTarget &RenderTarget) const
        {
            Message.DrawToWindow(RenderTarget);
            TextInput.DrawToWindow(RenderTarget);
        }

        AntiKeylogger &operator=(const AntiKeylogger &ItemCopy)
        {
            ERRORCLASSFUNCTION(AntiKeylogger,operator=,!SetAntiKeylogger(ItemCopy),RETURN_THIS)
            return *this;
        }
};

}

#endif
