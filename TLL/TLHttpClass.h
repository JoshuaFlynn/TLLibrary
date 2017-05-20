#ifndef G_TLHTTPCLASS_H
#define G_TLHTTPCLASS_H

/*
Generated on: 2013-08-22-19.40.17

Version: 1.4
*/

/*
Notes:
Added AppendHeaderField

*/

/*
Suggestions for improvements:

*/

#include <SFML/Network.hpp>
#include "CharArray.h"
#include "CommonFunctions.h"
#include <utility>
#include "TemplateList.h"
#include "OverWriter.h"
#include "StringTokeniser.h"
#include "FileProcEquals.h"
#include "Maths.h"
#include "Base64.h"

#define HEADER_DEFAULT 17

#define TEMPLATEHEADER "GET / HTTP/1.1\r\nAccept-Language: en-gb,en;q=0.8;\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*\057*;q=0.8\r\nUser-Agent: TLLibrary\r\nDNT: 1\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\n"

bool SEND = false;

namespace TL
{

extern const CharArray HeaderOptions[] = {"GET","POST","PUT","HEAD","TRACE","DELETE", "SEARCH", "CONNECT", "PROPFIND", "PROPPATCH", "PATCH", "MKCOL", "COPY", "MOVE", "LOCK", "UNLOCK", "OPTIONS"};

const TemplateArray<CharArray> GetLink(const CharArray &Link)
{
    ERRORFUNCTION(GetLink,!Link.IsValid(),return TemplateArray<CharArray>();)

    const char * HTTP = NULL;

    if(Find(Link.GetArray(),Link.GetSize()-1,"http://",7,false))
    {
        HTTP = Link.GetArray()+7;
    }
    else if(Find(Link.GetArray(),Link.GetSize()-1,"https://",8,false))
    {
        HTTP = Link.GetArray()+8;
    }
    else
    {
        HTTP = Link.GetArray();
    }


    const char * Temp = NULL;
    const char * End = Link.GetArray()+Link.GetSize();
    ERRORFUNCTION(GetLink,(Temp = FindItem(HTTP,End,'.')) == NULL,return TemplateArray<CharArray>();)

    TemplateArray<CharArray> URL;

    ERRORFUNCTION(GetLink,!URL.SetSize(2),return TemplateArray<CharArray>();)

    const char * Temp2 = NULL;

    if((Temp2 = FindItem(Temp,End,'/')) != NULL)
    {
        ERRORFUNCTION(GetLink,!URL[0].SetArray(HTTP,Temp2-HTTP),return TemplateArray<CharArray>();)
        ERRORFUNCTION(GetLink,!URL[1].SetArray(Temp2),return TemplateArray<CharArray>();)
        return URL;
    }

    if((Temp2 = FindItem(Temp,End,'\\')) != NULL)
    {
        ERRORFUNCTION(GetLink,!URL[0].SetArray(HTTP,Temp2-HTTP),return TemplateArray<CharArray>();)
        ERRORFUNCTION(GetLink,!URL[1].SetArray(Temp2),return TemplateArray<CharArray>();)
        return URL;
    }

    if((Temp2 = FindItem(Temp,End,'\0')) != NULL)
    {
        ERRORFUNCTION(GetLink,!URL[0].SetArray(HTTP),return TemplateArray<CharArray>();)
        ERRORFUNCTION(GetLink,!URL[1].SetArray("/"),return TemplateArray<CharArray>();)
        return URL;
    }
    return TemplateArray<CharArray>();
}

const SIZE_TYPE GetHeaderLength(const CharArray &Header)
{
    ERRORFUNCTION(GetHeaderLength,!Header.IsValid(),return 0;)
    ERRORFUNCTION(GetHeaderLength,Header.GetSize() < 10, return 0;)
    CharArray Temp;

    ERRORFUNCTION(GetHeaderLength,!Temp.SetSize(3),return 0;)
    Temp[0] = Header[0];
    Temp[1] = Header[1];
    Temp[2] = NULL;

    ERRORFUNCTION(GetHeaderLength,!Find(Header.GetArray(),Header.GetSize(),"\r\n\r\n",4,false),return 0;)
    SIZE_TYPE Temp2 = 0;
    ERRORFUNCTION(GetHeaderLength,(Temp2 = MeasureBetweenPoints(Header.GetArray(),Header.GetSize()-1,Temp.GetArray(),2,"\r\n\r\n",4,true)) == 0,return 0;)
    return Temp2;
}

const SIZE_TYPE GetPostContentLength(const CharArray &Header)
{
    ERRORFUNCTION(GetPostContentLength,!Header.IsValid(),return 0;)
    ERRORFUNCTION(GetPostContentLength,!Header.GetSize() < 10, return 0;)

    SIZE_TYPE Content = 0;

    ERRORFUNCTION(GetPostContentLength,(Content = GetHeaderLength(Header)) == 0, return 0;)
    return Header.GetSize()-Content;
}

const CharArray SendRaw(const CharArray &Host, const CharArray &POST, const short Port = 80, const float Timeout = 5.0)
{
    ERRORFUNCTION(SendRaw,!POST.IsValid(),return CharArray();)
    ERRORFUNCTION(SendRaw,!Host.IsValid(),return CharArray();)
    CharArray ReceivedStr;
    // Prepare the response
    sf::SocketTCP myConnection;

    // Connect the socket to the host
    ERRORFUNCTION(SendRaw,myConnection.Connect(Port, sf::IPAddress(Host.GetArray()), Timeout) != sf::Socket::Done,return CharArray();)
    ERRORFUNCTION(SendRaw,myConnection.Send(POST.GetArray(), POST.GetSize()-1) != sf::Socket::Done,return CharArray();)

    // Wait for the server's response
    std::size_t Size = 1;
    char Buffer[2048];

    sf::SelectorTCP CheckConnection;

    CheckConnection.Add(myConnection);

    while(CheckConnection.Wait(Timeout) > 0)
    {
        if(myConnection.Receive(Buffer, sizeof(Buffer), Size) == sf::Socket::Done)
        {
            ERRORFUNCTION(SendRaw,!ReceivedStr.Append(Buffer,Size-1),return CharArray();)
        }
        else
        {
            break;
        }
    }

    CheckConnection.Remove(myConnection);
    myConnection.Close();

    return ReceivedStr;
}

class TLHttpClass
{
	private:
	protected:
        uint8_t PostType;
        TemplateList< std::pair<CharArray,CharArray> > HeaderFields;
        CharArray Body;

        void Clear(){PostType = HEADER_DEFAULT;}
	public:

		TLHttpClass(){Open();}
		TLHttpClass(const TLHttpClass &ItemCopy){Open(); SetTLHttpClass(ItemCopy);}
		~TLHttpClass(){Close();}

        void Open(){Clear(); HeaderFields.Open(); Body.Open();}
        void Close(){Clear(); HeaderFields.Close(); Body.Close();}
        void Reset(){Open(); Close();}

        //This can return valid even if body is empty as it's possible to send just a header
        //This just checks it's merely in a valid state (EG contains some data), not if the post is HTTP compliant
        const bool IsValid() const {return HeaderFields.IsValid() && (PostType < HEADER_DEFAULT && PostType >= 0) && (Body.IsEmpty() || Body.IsValid());}
        const bool IsEmpty() const {return HeaderFields.IsEmpty() && PostType == HEADER_DEFAULT && Body.IsEmpty();}

        void TransferFrom(TLHttpClass &ItemCopy)
        {
            HeaderFields.TransferFrom(ItemCopy.HeaderFields);
            Body.TransferFrom(ItemCopy.Body);
            PostType = ItemCopy.PostType;
            ItemCopy.PostType = HEADER_DEFAULT;
            return;
        }

        const bool SetTLHttpClass(const TLHttpClass &ItemCopy)
        {
            TLHttpClass ItemCopy2;

            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!ItemCopy2.SetHeaderFields(ItemCopy.GetHeaderFields()),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!ItemCopy2.SetBody(ItemCopy.GetBody()),RETURN_BOOL)
            ItemCopy2.PostType = ItemCopy.PostType;
            TransferFrom(ItemCopy2);
            return true;
        }

        const bool SetTLHttpClass(const CharArray &Post)
        {
            ERRORCLASSFUNCTION(TlHttpClass,SetTLHttpClass,!Post.IsValid(),RETURN_BOOL)
            TLHttpClass ItemCopy;

            SIZE_TYPE Iter = GetHeaderLength(Post);

            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,Iter < 1,RETURN_BOOL)
            if(Post.GetSize() > Iter+1)
            {
                ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!ItemCopy.Body.SetArray(Post.GetArray()+Iter,Post.GetSize()-Iter),RETURN_BOOL)
            }

            CharArray Temp;

            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!Temp.SetArray(ExtractSubString(Post.GetArray(),Post.GetSize(),Post.GetArray(),2," ",1,false)),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!ItemCopy.SetPostType(Temp.GetArray(),Temp.GetSize()-2),RETURN_BOOL)

            StringTokeniser Temp2;

            const char * Temp3 = Find(Post.GetArray(),Post.GetSize(),"\r\n",2,true);

            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!Temp3,RETURN_BOOL)
            Temp3++;

            //What is this madness?
            //Search past the initial non-header field to the first available field
            //Truncate off the post
            //Add one for the null
            //Then shorten the truncation by however much forward we moved to skip the search past the initial non-header
            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!Temp.SetArray(Temp3,Iter+1-(Temp3-Post.GetArray())),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!Temp2.SetString(Temp),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!Temp2.SetSplitChars("\r\n"),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!Temp2.TokeniseString(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,Temp2.GetNumberOfTokens() < 1, RETURN_BOOL)

            do
            {
                ERRORCLASSFUNCTION(TLHttpClass,SetTLHttpClass,!ItemCopy.AddHeaderField(Temp2.GetCurrString()),RETURN_BOOL)
            }while(Temp2.ScanRight());

            TransferFrom(ItemCopy);
            return true;
        }

        const TemplateList<std::pair<CharArray,CharArray> > GetHeaderFields() const {return HeaderFields;}
        const CharArray GetBody() const {return Body;}
        const uint8_t GetPostType() const {return PostType;}

        const bool LoadHeaderTemplate(const uint8_t ItemCopy = HEADER_DEFAULT)
        {
            ERRORCLASSFUNCTION(TLHttpClass,LoadHeaderTemplate,ItemCopy < 0 || ItemCopy > HEADER_DEFAULT,RETURN_BOOL)
            if(ItemCopy == HEADER_DEFAULT)
            {
                return SetTLHttpClass(TEMPLATEHEADER);
            }

            TLHttpClass Temp;

            ERRORCLASSFUNCTION(TLHttpClass,LoadHeaderTemplate,!Temp.SetTLHttpClass(TEMPLATEHEADER),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,LoadHeaderTemplate,!Temp.SetPostType(ItemCopy),RETURN_BOOL)
            TransferFrom(Temp);
            return true;
        }

        const bool LoadBase64HeaderTemplate(const CharArray &Template)
        {
            ERRORCLASSFUNCTION(TLHttpClass,LoadBase64HeaderTemplate,!Template.IsValid(),RETURN_BOOL)

            CharArray Decoded;
            ERRORCLASSFUNCTION(TLHttpClass,LoadBase64HeaderTemplate,!Decoded.SetArray(Base64().Decode(Template)),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,LoadBase64HeaderTemplate,!Decoded.IsValid(),RETURN_BOOL)
            return SetTLHttpClass(Decoded);
        }

        const bool SetHeaderFields(const TemplateList<std::pair<CharArray,CharArray> > &ItemCopy)
        {
            ERRORCLASSFUNCTION(TLHttpClass,SetHeaderFields,!HeaderFields.Assign(ItemCopy),RETURN_BOOL)
            return true;
        }

        const bool SetPostType(const char ItemCopy[], const SIZE_TYPE IC1)
        {
            ERRORCLASSFUNCTION(TLHttpClass,SetPostType,!ItemCopy,RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetPostType,IC1 < 1,RETURN_BOOL)

            uint8_t Iter = 0;

            while(Iter < HEADER_DEFAULT)
            {
                if(Compare(ItemCopy,IC1,HeaderOptions[Iter].GetArray(),HeaderOptions[Iter].GetSize()-1,true) )
                {
                    return SetPostType(Iter);
                }
                Iter++;
            }
            ERRORCLASSFUNCTION(TLHttpClass,SetPostType,Iter >= HEADER_DEFAULT,RETURN_BOOL)
            return false;
        }

        const bool SetPostType(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(TLHttpClass,SetPostType,!ItemCopy.IsValid(),RETURN_BOOL)
            return SetPostType(ItemCopy.GetArray(),ItemCopy.GetSize());
        }

        const bool SetPostType(const uint8_t &ItemCopy)
        {
            ERRORCLASSFUNCTION(TLHttpClass,SetPostType,ItemCopy > HEADER_DEFAULT,RETURN_BOOL)
            PostType = ItemCopy;
            return true;
        }

        const bool SetBody(const CharArray &ItemCopy)
        {
            ERRORCLASSFUNCTION(TLHttpClass,SetBody,!Body.SetArray(ItemCopy),RETURN_BOOL)
            return true;
        }

        const bool AppendHeaderField(const CharArray &Header)
        {
            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!Header.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!Find(Header.GetArray(),Header.GetSize(),": ",2,false),RETURN_BOOL)

            StringTokeniser Temp;

            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!Temp.SetSplitChars(":\r\n"),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!Temp.SetString(Header),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!Temp.TokeniseString(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,Temp.GetNumberOfTokens() != 2, RETURN_BOOL)

            const char * Temp2 = Temp.GetCurrString();

            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!Temp2,RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!Temp.ScanRight(),RETURN_BOOL)
            return AppendHeaderField(Temp2,Temp.GetCurrString() + 1); //+1 to skip the space
        }

        const bool AppendHeaderField(const CharArray &Header, const CharArray &Field)
        {
            TemplateIteratorNonConst<std::pair<CharArray,CharArray> > Temp = HeaderFields;

            if(Temp.IsValid())
            {
                Temp.CurrToStart();
                do
                {
                    if(Compare(Header.GetArray(),Header.GetSize(),Temp.GetCurrItem().first.GetArray(),Temp.GetCurrItem().first.GetSize()))
                    {
                        ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!Temp.GetCurrItem().second.Append(Field),RETURN_BOOL)
                        return true;
                    }
                }while(Temp.CurrNext());
            }

            CharArray ItemCopy[2];

            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!ItemCopy[0].SetArray(Header),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!ItemCopy[1].SetArray(Field),RETURN_BOOL)

            ERRORCLASSFUNCTION(TLHttpClass,AppendHeaderField,!HeaderFields.AddNodeEndNext(),RETURN_BOOL)

            HeaderFields.GetEndItem().first.TransferFrom(ItemCopy[0]);
            HeaderFields.GetEndItem().second.TransferFrom(ItemCopy[1]);
            return true;
        }

        const bool AddHeaderField(const CharArray &Header)
        {
            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Header.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Find(Header.GetArray(),Header.GetSize(),": ",2,false),RETURN_BOOL)

            StringTokeniser Temp;

            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Temp.SetSplitChars("\r\n"),RETURN_BOOL)


            CharArray Temp3;

            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Temp3.SetArray(Header),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Temp3.Replace(": ",2,"\0 ",2),RETURN_BOOL)

            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Temp.SetString(Temp3),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Temp.TokeniseString(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,Temp.GetNumberOfTokens() != 2, RETURN_BOOL)

            const char * Temp2 = Temp.GetCurrString();

            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Temp2,RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Temp.ScanRight(),RETURN_BOOL)
            return AddHeaderField(Temp2,Temp.GetCurrString() + 1); //+1 to skip the space
        }

        const bool AddHeaderField(const CharArray &Header, const CharArray &Field)
        {
            TemplateIteratorNonConst<std::pair<CharArray,CharArray> > Temp = HeaderFields;

            if(Temp.IsValid())
            {
                Temp.CurrToStart();
                do
                {
                    if(Compare(Header.GetArray(),Header.GetSize(),Temp.GetCurrItem().first.GetArray(),Temp.GetCurrItem().first.GetSize()))
                    {
                        ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!Temp.GetCurrItem().second.SetArray(Field),RETURN_BOOL)
                        return true;
                    }
                }while(Temp.CurrNext());
            }

            CharArray ItemCopy[2];

            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!ItemCopy[0].SetArray(Header),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!ItemCopy[1].SetArray(Field),RETURN_BOOL)

            ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!HeaderFields.AddNodeEndNext(),RETURN_BOOL)

            HeaderFields.GetEndItem().first.TransferFrom(ItemCopy[0]);
            HeaderFields.GetEndItem().second.TransferFrom(ItemCopy[1]);
            return true;
        }

        const bool AddHeaderField(const std::pair<CharArray,CharArray> &ItemCopy)
        {
            return AddHeaderField(ItemCopy.first,ItemCopy.second);
        }

        const bool RemoveHeaderField(const CharArray &Header)
        {
            if(HeaderFields.IsValid())
            {
                HeaderFields.CurrToStart();
                do
                {
                    if(Compare(Header.GetArray(),Header.GetSize(),HeaderFields.GetCurrItem().first.GetArray(),HeaderFields.GetCurrItem().first.GetSize()))
                    {
                        ERRORCLASSFUNCTION(TLHttpClass,AddHeaderField,!HeaderFields.RemoveNodeCurr(),RETURN_BOOL)
                        HeaderFields.CurrToStart();
                        return true;
                    }
                }while(HeaderFields.CurrNext());
            }

            return false;
        }

        const std::pair<CharArray,CharArray> FindHeader(const CharArray &Header) const
        {
            TemplateIterator<std::pair<CharArray,CharArray> > Temp = HeaderFields;
            if(Temp.IsEmpty())
            {
                return std::pair<CharArray,CharArray>();
            }

            Temp.CurrToStart();
            do
            {
                //if(Compare(Header.GetArray(),Header.GetSize(),Temp.GetCurrItem().first.GetArray(),Temp.GetCurrItem().first.GetSize()))
                if(Header == Temp.GetCurrItem().first)
                {
                    return Temp.GetCurrItem();
                }
            }while(Temp.CurrNext());

            return std::pair<CharArray,CharArray>();
        }

        const std::pair<CharArray,CharArray> FindHeaderField(const CharArray &HeaderField) const
        {
            TemplateIterator<std::pair<CharArray,CharArray> > Temp = HeaderFields;
            if(Temp.IsEmpty())
            {
                return std::pair<CharArray,CharArray>();
            }

            Temp.CurrToStart();
            do
            {
                if(Compare(HeaderField.GetArray(),HeaderField.GetSize(),Temp.GetCurrItem().second.GetArray(),Temp.GetCurrItem().second.GetSize()))
                {
                    return Temp.GetCurrItem();
                }
            }while(Temp.CurrNext());

            return std::pair<CharArray,CharArray>();
        }

        //This should not be the number of arrays, but the number of partitions
        const bool SetMultipartPost(const CharArray &WebBoundary, const SIZE_TYPE NumberOfArrays, ...)
        {
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!WebBoundary.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,NumberOfArrays < 1,RETURN_BOOL)

            CharArray WebKitBoundary, Return, ContentDisposition[2], ItemCopy;

            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!WebKitBoundary.AppendAll(3,"------WebKitFormBoundary",WebBoundary.GetArray(),"\r\n"),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!Return.SetArray("\r\n"),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ContentDisposition[0].SetArray("Content-Disposition: form-data; name=\""),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ContentDisposition[1].SetArray("\""),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy.AppendAll(3,"------WebKitFormBoundary",WebBoundary.GetArray(),"--\r\n\r\n"),RETURN_BOOL)

            TemplateList<std::pair<const char *, SIZE_TYPE> > ItemCopy2;

            va_list valist;
            va_start(valist,NumberOfArrays);

            const char * ItemCopy3 = NULL;

            SIZE_TYPE Iter = 0;
            while(Iter < NumberOfArrays)
            {
                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
                ItemCopy2.GetEndItem().first = WebKitBoundary.GetArray();
                ItemCopy2.GetEndItem().second = WebKitBoundary.GetSize()-1;
                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
                ItemCopy2.GetEndItem().first = ContentDisposition[0].GetArray();
                ItemCopy2.GetEndItem().second = ContentDisposition[0].GetSize()-1;

                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
                ItemCopy2.GetEndItem().first = va_arg(valist,const char *);
                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!(ItemCopy2.GetEndItem().first),RETURN_BOOL)
                ItemCopy2.GetEndItem().second = strlen(ItemCopy2.GetEndItem().first);
                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,ItemCopy2.GetEndItem().second < 1,RETURN_BOOL)

                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
                ItemCopy2.GetEndItem().first = ContentDisposition[1].GetArray();
                ItemCopy2.GetEndItem().second = ContentDisposition[1].GetSize()-1;

                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
                ItemCopy2.GetEndItem().first = Return.GetArray();
                ItemCopy2.GetEndItem().second = Return.GetSize()-1;
                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
                ItemCopy2.GetEndItem().first = Return.GetArray();
                ItemCopy2.GetEndItem().second = Return.GetSize()-1;

                ItemCopy3 = va_arg(valist,const char *);

                if(ItemCopy3)
                {
                    ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
                    ItemCopy2.GetEndItem().first = ItemCopy3;
                    ItemCopy2.GetEndItem().second = strlen(ItemCopy3);
                    ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,ItemCopy2.GetEndItem().second < 1,RETURN_BOOL)
                }

                ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
                ItemCopy2.GetEndItem().first = Return.GetArray();
                ItemCopy2.GetEndItem().second = Return.GetSize()-1;
                Iter++;
            }

            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!ItemCopy2.AddNodeEndNext(),RETURN_BOOL)
            ItemCopy2.GetEndItem().first = ItemCopy.GetArray();
            ItemCopy2.GetEndItem().second = ItemCopy.GetSize();

            TemplateArray<const char *> Itemcopy;
            TemplateArray<SIZE_TYPE> Itemcopy2;


            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!Itemcopy.SetSize(ItemCopy2.GetSize()),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!Itemcopy2.SetSize(ItemCopy2.GetSize()),RETURN_BOOL)
            ItemCopy2.CurrToStart();
            do
            {
                Itemcopy[ItemCopy2.GetIter()-1] = ItemCopy2.GetCurrItem().first;
                Itemcopy2[ItemCopy2.GetIter()-1] = ItemCopy2.GetCurrItem().second;
            }while(ItemCopy2.CurrNext());

            CharArray Itemcopy3;

            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!Itemcopy3.AppendAll(Itemcopy,Itemcopy2),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!WebKitBoundary.SetArray(WebKitBoundary.GetArray()+2,WebKitBoundary.GetSize()-5),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!WebKitBoundary.Insert(0,"multipart/form-data; boundary="),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,SetMultipartPost,!AddHeaderField("Content-Type",WebKitBoundary) ,RETURN_BOOL)

            Body.TransferFrom(Itemcopy3);
            return true;
        }

        const bool EncodeToBase64()
        {

            ERRORCLASSFUNCTION(TLHttpClass,EncodeToBase64,!Body.IsValid(),RETURN_BOOL)

            CharArray ItemCopy;
            ERRORCLASSFUNCTION(TLHttpClass,EncodeToBase64,!ItemCopy.SetArray(Base64().Encode(Body.GetArray(),Body.GetSize()-1,0)),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,EncodeToBase64,!AddHeaderField("Fiddler-Encoding","base64"),RETURN_BOOL)
            Body.TransferFrom(ItemCopy);
            return true;
        }

        const bool DecodeFromBase64()
        {
            ERRORCLASSFUNCTION(TLHttpClass,EncodeToBase64,!Body.IsValid(),RETURN_BOOL)

            CharArray ItemCopy;
            ERRORCLASSFUNCTION(TLHttpClass,EncodeToBase64,!ItemCopy.SetArray(Base64().Decode(Body.GetArray(),Body.GetSize()-1)),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,EncodeToBase64,!RemoveHeaderField("Fiddler-Encoding"),RETURN_BOOL)
            Body.TransferFrom(ItemCopy);
            return true;
        }

        const bool AppendFileUpload(const CharArray &Name, const CharArray &ContentType, const CharArray &Filename = CharArray(), const bool TruncateFilename = true)
        {
            //Checks is valid
            ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!Body.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!ContentType.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!Name.IsValid(),RETURN_BOOL)

            //For some bullshit pointer work
            const char * ItemCopy = NULL;

            //Find the end of body (Which ironically should be on the end)
            ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!(ItemCopy = Find(Body.GetArray(),Body.GetSize(),"--\r\n\r\n\0",7,false)),RETURN_BOOL)

            //Scan backwards until the start of the last webboundary
            ItemCopy = FindItemBackwards(Body.GetArray(),(ItemCopy-Body.GetArray()),'\n');
            ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!ItemCopy,RETURN_BOOL)
            ItemCopy = ItemCopy + 1;

            CharArray WebBoundary,WebKitBoundary, Filename2;
            TemplateArray<char> File;

            //I have no fucking idea what ItemCopy2 is for
            ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!WebKitBoundary.SetArray(ItemCopy),RETURN_BOOL)
            if(!Filename.IsValid())
            {
                return Body.SetArrayAll(8,Body.GetArray(),Body.GetSize()-WebKitBoundary.GetSize(),WebKitBoundary.GetArray(),WebKitBoundary.GetSize()-7,"\r\nContent-Disposition: form-data; name=\"",40,Name.GetArray(),Name.GetSize()-1,"\"; filename=\"\"\r\nContent-Type: ",30,ContentType.GetArray(),ContentType.GetSize()-1,"\r\n\r\n\r\n",6,WebKitBoundary.GetArray(),WebKitBoundary.GetSize());
            }
            else
            {
                ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!File.SetArray(FileProcEquals(Filename,"rb").ToTemplateArray()),RETURN_BOOL)
                ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!File.IsValid(),RETURN_BOOL)

                if(TruncateFilename)
                {
                    ItemCopy = FindItemBackwards(Filename.GetArray(),Filename.GetSize(),'\\');
                    ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!ItemCopy,RETURN_BOOL)
                    ItemCopy = ItemCopy + 1;
                    ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!Filename2.SetArray(ItemCopy),RETURN_BOOL)
                }
                else
                {
                    ERRORCLASSFUNCTION(TLHttpClass,AppendFileUpload,!Filename2.SetArray(Filename),RETURN_BOOL)
                }
                return Body.SetArrayAll(12,Body.GetArray(),Body.GetSize()-WebKitBoundary.GetSize(),WebKitBoundary.GetArray(),WebKitBoundary.GetSize()-7,"\r\nContent-Disposition: form-data; name=\"",40,Name.GetArray(),Name.GetSize()-1,"\"; filename=\"",13,Filename2.GetArray(),Filename2.GetSize()-1,"\"\r\nContent-Type: ",17,ContentType.GetArray(),ContentType.GetSize()-1,"\r\n\r\n",4,File.GetArray(),File.GetSize(),"\r\n",2,WebKitBoundary.GetArray(),WebKitBoundary.GetSize());
            }
            return false;
        }

        const bool BodyReplace(const CharArray &Find, const CharArray &ReplaceWith)
        {
            ERRORCLASSFUNCTION(TLHttpClass,BodyReplace,!Body.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,BodyReplace,!Find.IsValid(),RETURN_BOOL)
            ERRORCLASSFUNCTION(TLHttpClass,BodyReplace,!ReplaceWith.IsValid(),RETURN_BOOL)

            return Body.Replace(Find.GetArray(),Find.GetSize()-1,ReplaceWith.GetArray(),ReplaceWith.GetSize()-1);
        }

        const CharArray GetRawPost(const CharArray &Link, const bool Update = true, const bool DNT = true)
        {
            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!IsValid(),return CharArray();)
            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Link.IsValid(),return CharArray();)
            CharArray Post;
            TemplateArray<CharArray> URL;

            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!URL.SetArray(GetLink(Link)),return CharArray();)
            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!URL.IsValid(),return CharArray();)

            SIZE_TYPE Total = 0;

            Total = Total + HeaderOptions[PostType].GetSize()-1; //GET etc
            Total = Total + 1; //1 for the space
            Total = Total + URL[1].GetSize(); //SUBURL for page request
            Total = Total + 11; //1 for space, 8 HTTP/1.1, 2 for /r/n

            if(Update)
            {
                if(Body.IsValid())
                {
                    ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!AddHeaderField("Content-Length",IntToString(Body.GetSize()-1)),return CharArray();)
                }
                else
                {
                    if(FindHeader("Content-Length").first.IsValid())
                    {
                        ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!AddHeaderField("Content-Length","0"),return CharArray();)
                    }
                }
                //Append Host
                ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!AddHeaderField("Host",URL[0]),return CharArray();)
            }

            if(DNT)
            {
                ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!AddHeaderField("DNT","1"),return CharArray();)
            }

            HeaderFields.CurrToStart();
            do
            {
                Total = Total + HeaderFields.GetCurrItem().first.GetSize()-1;//-1 to discount the null on the end
                Total = Total + 2; //1 for the colon, 1 for the space
                Total = Total + HeaderFields.GetCurrItem().second.GetSize()-1;//-1 to discount the null
                Total = Total + 2;//1 for \r, 1 for \n
            }while(HeaderFields.CurrNext());
            Total = Total + 2;//1 for \r, 1 for \n which denotes the end of the header

            if(Body.IsValid())
            {
                Total = Total + Body.GetSize()-1;//-1 for excluded null
            }

            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Post.SetSize(Total),return CharArray();)

            OverWriter<char> Updater;
            Updater.SetArray(Post.GetArray(),Post.GetSize());

            //Write header type
            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite(HeaderOptions[PostType].GetArray(),HeaderOptions[PostType].GetSize()-1),return CharArray();)
            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite(' '),return CharArray();)


            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite(URL[1].GetArray(),URL[1].GetSize()-1),return CharArray();)
            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite(' '),return CharArray();)
            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite("HTTP/1.1\r\n",10),return CharArray();)

            HeaderFields.CurrToStart();
            do
            {
                ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite(HeaderFields.GetCurrItem().first.GetArray(),HeaderFields.GetCurrItem().first.GetSize()-1),return CharArray();)
                ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite(": ",2),return CharArray();)
                ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite(HeaderFields.GetCurrItem().second.GetArray(),HeaderFields.GetCurrItem().second.GetSize()-1),return CharArray();)
                ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite("\r\n",2),return CharArray();)
            }while(HeaderFields.CurrNext());

            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite("\r\n",2),return CharArray();)

            if(Body.IsValid())
            {
                ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite(Body.GetArray(),Body.GetSize()-1),return CharArray();)
            }

            ERRORCLASSFUNCTION(TLHttpClass,GetRawPost,!Updater.OverWrite('\0'),return CharArray();)

            return Post;
        }


        //Update will update basic variables like 'content-length' if applicable
        const CharArray Send(const CharArray &Link, const float TimeOut = 10.0, const bool Update = true, const bool DNT = true)
        {
            ERRORCLASSFUNCTION(TLHttpClass,Send,!Link.IsValid(),return CharArray();)
            TemplateArray<CharArray> URL;
            ERRORCLASSFUNCTION(TLHttpClass,Send,!URL.SetArray(GetLink(Link)),return CharArray();)
            ERRORCLASSFUNCTION(TLHttpClass,Send,!URL.IsValid(),return CharArray();)

            return SendRaw(URL[0],GetRawPost(Link,Update,DNT),80,TimeOut);
        }

        TLHttpClass &operator=(const TLHttpClass &ItemCopy)
        {
            ERRORCLASSFUNCTION(TLHttpClass,operator=,!SetTLHttpClass(ItemCopy),RETURN_THIS)
            return *this;
        }
};

}
#undef TEMPLATEHEADER
#undef HEADER_DEFAULT
#endif
