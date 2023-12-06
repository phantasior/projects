#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <vector>

const uint8_t HEADER_FLAGS_SIZE = 1;
const uint8_t ENCODING_SIZE = 1;
const uint8_t FLAGS_SIZE = 2;
const uint8_t HEADER_VERSION_SIZE = 2;
const uint8_t HEADER_FILE_ID_SIZE = 3;
const uint8_t LANGUAGE_SIZE = 3;
const uint8_t FRAME_ID_SIZE = 4;
const uint8_t DATE_SIZE = 8;

struct Header {
    Header() : unsync(false), ext_header(false), exp_ind(false), footer(false), size(0) {
        file_id.resize(HEADER_FILE_ID_SIZE);
        version.resize(HEADER_VERSION_SIZE);
    }

    std::string file_id;
    std::string version;
    bool unsync;
    bool ext_header;
    bool exp_ind;
    bool footer;
    size_t size;
};

struct ExtHeader {
    ExtHeader()
        : size(0), num_of_flag_bytes(0x00), tag_is_upd(false)
        , crc(false), restriction(false), tag_size_restriction(false)
        , text_enc_restriction(false), text_field_size_restriction(false)
        , img_enc_restriction(false), img_size_restriction(false)
        , flags(0x00), total_frame_crc(0), flag_data_length(0x00)
    {}
    size_t size; // Extended header size
    size_t total_frame_crc; // Noone know what is it
    char num_of_flag_bytes; // Number of flag bytes
    char flags; // Flags
    bool tag_is_upd; // Tag is an update
    bool crc; // CRC data present
    bool restriction; // Tag restrictionictions
    bool tag_size_restriction; // Tag size restrictionictions
    bool text_enc_restriction; // Text encoding restrictionictions
    bool text_field_size_restriction; // Text fields size restrictionictions
    bool img_enc_restriction; // Image encoding restrictionictions
    bool img_size_restriction; // Image size restrictionictions
    char flag_data_length; // Flag about size of data
};

void Parse(const std::string& file);

void ReadData(char encoding, std::ifstream& in, std::string& data);

Header ReadHeader(std::ifstream& in);

ExtHeader ReadExtHeader(std::ifstream& in);

size_t ReadSize(std::ifstream& in);

uint32_t GetTime(std::ifstream& in);

std::string ReadDataToZeroByte(std::ifstream& in, size_t encoding);

std::string ISO_8859_TO_UTF_8(const std::string& str);

bool IsBitSet(char chr, size_t bit);

inline std::string MapEncodingToText(size_t encoding) noexcept {
    switch (encoding) {
    case 0x00:
        return "ISO-8859-1 [ISO-8859-1]. Terminated with $00.";
    case 0x01:
        return "UTF-16 [UTF-16] encoded Unicode [UNICODE] with BOM. All \
            strings in the same frame SHALL have the same byteorder. \
            Terminated with $00 00.";
    case 0x02:
        return "UTF-16BE [UTF-16] encoded Unicode [UNICODE] without BOM. \
            Terminated with $00 00.";
    case 0x03:
        return "UTF-8 [UTF-8] encoded Unicode [UNICODE]. Terminated with $00.";
    default:
        return "Incorrect encoding.";
    }
}

inline std::string MapEventToDescription(size_t event) {
    switch (event) {
    case 0x00:
        return "padding (has no meaning)";
    case 0x01:
        return "end of initial silence";
    case 0x02:
        return "intro start";
    case 0x03:
        return "main part start";
    case 0x04:
        return "outro start";
    case 0x05:
        return "outro end";
    case 0x06:
        return "verse start";
    case 0x07:
        return "refrain start";
    case 0x08:
        return "interlude start";
    case 0x09:
        return "theme start";
    case 0x0A:
        return "variation start";
    case 0x0B:
        return "key change";
    case 0x0C:
        return "time change";
    case 0x0D:
        return "momentary unwanted noise (Snap, Crackle & Pop)";
    case 0x0E:
        return "sustained noise";
    case 0x0F:
        return "sustained noise end";
    case 0x10:
        return "intro end";
    case 0x11:
        return "main part end";
    case 0x12:
        return "verse end";
    case 0x13:
        return "refrain end";
    case 0x14:
        return "theme end";
    case 0x15:
        return "profanity";
    case 0x16:
        return "profanity end";
    case 0xFD:
        return "audio end (start of silence)";
    case 0xFE:
        return "audio file ends";
    case 0xFF:
        return "one more byte of events follows (all the following bytes with \
                the value $FF have the same function)";
    default:
        break;
    }

    if (0x17 <= event && event <= 0xDF) {
        return "reserved for future use";
    } else if (0xE0 <= event && event <= 0xEF) {
        return "not predefined synch 0-F";
    } else if (0xF0 <= event && event <= 0xFC) {
        return "reserved for future use";
    }

    return "unknown event";
}

class Frame {
public:
    Frame(std::ifstream& in) : size(ReadSize(in)) {
        flags.resize(FLAGS_SIZE);
        in.read(flags.data(), flags.size());
    }

    virtual ~Frame() = default;

    size_t Size() const { return size + 10; }
    friend std::ifstream& operator>>(std::ifstream& in, Frame& frame);
    friend std::ostream& operator<<(std::ostream& out, const Frame& frame);
protected:
    virtual void Read(std::ifstream& in) = 0;
    virtual void Print(std::ostream& out) const = 0;
    std::string type;
    std::string flags;
    size_t size;
};

std::ifstream& operator>>(std::ifstream& in, Frame& frame);

std::ostream& operator<<(std::ostream& out, const Frame& frame);

// Base frame
class LanguageFrame : public Frame {
public:
    LanguageFrame(std::ifstream& in) : Frame(in) {
        language.resize(LANGUAGE_SIZE);
    }
protected:
    char encoding;
    std::string language;
    std::string desc;
    std::string data;
};

/*
<Header for 'Text information frame', ID: "T000" - "TZZZ",
excluding "TXXX" described in 4.2.6.>
Text encoding                $xx
Information                  <text string(s) according to encoding>
*/

class TextFrame: public Frame {
public:
    TextFrame(std::ifstream& in) : Frame(in) {
        type = "Text Frame";
    }

protected:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        if (encoding == 0x01) {
            in.seekg(ENCODING_SIZE, in.cur);
        }
        
        size_t cur_byte = 1; 
        std::string cur;
        while (cur_byte < size) {
            cur += in.get();
            cur_byte++;
            if (encoding == 0x03) {
                if (cur.size() >= 2 && cur.back() == 0x00 && cur[cur.size() - 2] == 0x00) {
                    data.push_back(cur);
                    cur.clear();
                }
            } else {
                if (cur.back() == 0x00) {
                    data.push_back(cur);
                    cur.clear();
                }
            }
        }

        if (cur.size() != 0) {
            data.push_back(cur);
        }
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Encoding is: " << MapEncodingToText(encoding) << '\n';
        out << "Size: " << size << '\n';
        out << "Content: \n";
        for (const auto& i : data) {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }

    char encoding;
    std::vector<std::string> data;
};

/*
<Header for 'User defined text information frame', ID: "TXXX">
Text encoding     $xx
Description       <text string according to encoding> $00 (00)
Value             <text string according to encoding>

*/

class TXXXFrame: public TextFrame {
public:
    TXXXFrame(std::ifstream& in) : TextFrame(in) {}
private:
    private:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        data.push_back(ReadDataToZeroByte(in, 0x03));
        value.resize(size - 1 - data[0].size() - 1);
        in.read(value.data(), value.size());
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Encoding is: " << MapEncodingToText(encoding) << '\n';
        out << "Content: " << data[0] << '\n';
        out << "Value: " << value << '\n';
        std::cout << '\n';
    }

    std::string value;
};

/*
<Header for 'Comment', ID: "COMM">
Text encoding          $xx
Language               $xx xx xx
Short content descrip. <text string according to encoding> $00 (00)
The actual text        <full text string according to encoding>
*/

class CommentFrame: public LanguageFrame {
public:
    CommentFrame(std::ifstream& in) : LanguageFrame(in) {
        type = "Comment Frame";
    }

private:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        in.read(language.data(), language.size());
        desc = ReadDataToZeroByte(in, encoding);
        data.resize(size - ENCODING_SIZE - language.size() - desc.size() - 1);
        ReadData(encoding, in, data);
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Encoding: " << MapEncodingToText(encoding) << '\n';
        out << "Language: " << language << '\n';
        out << "Description: " << desc << '\n';
        out << "Data: " << data << '\n' << '\n';
    }
};

/*
<Header for 'Popularimeter', ID: "POPM">
Email to user   <text string> $00
Rating          $xx
Counter         $xx xx xx xx (xx ...)
*/

class PopularimeterFrame: public Frame {
public:
    PopularimeterFrame(std::ifstream& in) : Frame(in) {
        type = "Popularimeter Frame";
    }

private:
    void Read(std::ifstream& in) override {
        email = ReadDataToZeroByte(in, 0x03);
        rating = in.get();
        counter.resize(size - email.size() - 1 - 1);
        in.read(counter.data(), counter.size());
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Email: " << email << '\n';
        out << "Rating: " << (int)rating << '\n';
        out << "Counter: " << counter << '\n' << '\n';
    }

    char rating;
    std::string email;
    std::string counter;
};

/*
<Header for 'Unsynchronised lyrics/text transcription', ID: "USLT">
Text encoding        $xx
Language             $xx xx xx
Content descriptor   <text string according to encoding> $00 (00)
Lyrics/text          <full text string according to encoding>
*/

class TranscriptionFrame: public LanguageFrame {
public:
    TranscriptionFrame(std::ifstream& in) : LanguageFrame(in) {
        type = "Transcription Frame";
    }

private:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        in.read(language.data(), language.size());
        desc = ReadDataToZeroByte(in, encoding);
        data.resize(size - 1 - language.size() - desc.size() - 1);
        ReadData(encoding, in, data);
    } 

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Language: " << language << '\n' ;
        out << "Content: " << desc << '\n' ;
        out << "Text: " << data << '\n' << '\n';
    }
};

/*
<Header for 'URL link frame', ID: "W000" - "WZZZ", excluding "WXXX" described in 4.3.2.>
URL              <text string>
*/

class URLFrame: public Frame {
public:
    URLFrame(std::ifstream& in) : Frame(in) {
        url.resize(size);
        type = "URL Frame";
    }

protected:
    void Read(std::ifstream& in) override {
        in.read(url.data(), url.size());
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "URL: " << url << '\n' << '\n';
    }

    std::string url;
};

/*
<Header for 'User defined URL link frame', ID: "WXXX">
Text encoding     $xx
Description       <text string according to encoding> $00 (00)
URL               <text string>
*/

class WXXXrame: public URLFrame {
public:
    WXXXrame(std::ifstream& in) : URLFrame(in) {
        type = "URL Frame";
    }

protected:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        desc = ReadDataToZeroByte(in, encoding);
        url.resize(size - ENCODING_SIZE - desc.size() - 1);
        in.read(url.data(), url.size());
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "URL: " << url << '\n' << '\n';
        out << "Description: " << desc << '\n' << '\n';
    }

    char encoding;
    std::string desc;
};

/*
<Header for 'Attached picture', ID: "APIC">
Text encoding      $xx
MIME type          <text string> $00
Picture type       $xx
Description        <text string according to encoding> $00 (00)
Picture data       <binary data>
*/

class PictureFrame: public Frame {
public:
    PictureFrame(std::ifstream& in, const std::string& file_) : Frame(in), file(file_) {
        type = "Picture Frame";
    }

private:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        mime_type = ReadDataToZeroByte(in, encoding);
        pic_type = in.get();
        desc = ReadDataToZeroByte(in, encoding);

        size_t delim_pos = mime_type.find('/');
        std::string type = delim_pos == std::string::npos ? ".undefined" : "." + mime_type.substr(delim_pos + 1);

        char byte;
        std::ofstream out(file + type, std::ios::binary);
        for (size_t i = 0; i < size - 1 - mime_type.size() - 1 - 1 - desc.size() - 1; ++i) {
            in.read(&byte, 1);
            out.write(&byte, 1);
        }
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Mime Type: " << mime_type << '\n';
        out << "Picture Type: " << pic_type << '\n';
        out << "Description: " << desc << '\n' << '\n';
    }
    
    char encoding;
    std::string mime_type;
    char pic_type;
    std::string desc;
    std::string file;
};

/*
<Header for 'Play counter', ID: "PCNT">
Counter        $xx xx xx xx (xx ...)
*/

class PlayCounterFrame: public Frame {
public:
    PlayCounterFrame(std::ifstream& in) : Frame(in) {
        type = "Play Counter Frame";
    }

private:
    void Read(std::ifstream& in) override {
        counter.resize(size);
        in.read(counter.data(), size);
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Counter: " << counter << '\n';
    }

    std::string counter;
};

/*
<Header for 'Private frame', ID: "PRIV">
Owner identifier      <text string> $00
The private data      <binary data>
*/

class PrivateFrame: public Frame {
public:
    PrivateFrame(std::ifstream& in) : Frame(in) {
        type = "Private Frame";
    }

private:
    void Read(std::ifstream& in) override {
        owner_id = ReadDataToZeroByte(in, 0x03);
        private_data.resize(size - owner_id.size() - 1);
        in.read(private_data.data(), private_data.size());
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Owner ID: " << owner_id << '\n';
    }

    std::string owner_id;
    std::string private_data;
};

/*
<Header for 'Group ID registration', ID: "GRID">
Owner identifier      <text string> $00
Group symbol          $xx
Group dependent data  <binary data>
*/

class GroupIdFrame: public Frame {
public:
    GroupIdFrame(std::ifstream& in) : Frame(in) {
        type = "Group ID Frame";
    }

private:
    void Read(std::ifstream& in) override {
        owner_id = ReadDataToZeroByte(in, 0x03);
        group_symbol = in.get();
        group_data.resize(size - owner_id.size() - 1);
        in.read(group_data.data(), group_data.size());
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Owner ID: " << owner_id << '\n';
        out << "Group symbol: " << group_symbol << '\n';
        out << "Group data: " << group_data << '\n';
    }

    std::string owner_id;
    char group_symbol;
    std::string group_data;
};

/*
<Header for 'Event timing codes', ID: "ETCO">
Time stamp format    $xx
    $01  Absolute time, 32 bit sized, using MPEG [MPEG] frames as unit
    $02  Absolute time, 32 bit sized, using milliseconds as unit
then pairs<event[1byte], time_stamp<4byte>> til frame end
*/

class ETCOFrame: public Frame {
public:
    ETCOFrame(std::ifstream& in) : Frame(in) {
        type = "ETCO Frame";
    }

private:
    void Read(std::ifstream& in) override {
        time_stamp_format = in.get();
        
        size_t cur_byte = 1; // already ream time stamp and type of event
        while (cur_byte < size) {
            char event;
            event = in.get();
            uint32_t time = GetTime(in);
            cur_byte += 5;
            data.push_back({event, time});
        }
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        for (const auto& i : data) {
            std::cout << MapEventToDescription(i.first) << ' ' << i.second << '\n';
        }
        std::cout << '\n';
    }

    char time_stamp_format;
    std::vector<std::pair<char, uint32_t>> data;
};

/*
    encoding 1 byte
    language 3 bytes
    time_stamp_format 1 byte
    content_type 1 byte
    content_descriptor 1byte

    (content[0x00]+4byte of timestamp) n times
    believe me
*/

class SYLTFrame: public LanguageFrame {
public:
    SYLTFrame(std::ifstream& in) : LanguageFrame(in) {
        type = "SYLT Frame";
    }

private:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        in.read(language.data(), language.size());
        time_stamp_format = in.get();
        content_type = in.get();
        desc = ReadDataToZeroByte(in, 0x03);

        size_t cur_byte = ENCODING_SIZE + LANGUAGE_SIZE + 1 + 1 + desc.size() + 1;
        char byte;
        while (cur_byte < size) {
            std::string lyrics = ReadDataToZeroByte(in, 0x03);
            uint32_t time = GetTime(in);

            time_data.push_back({time, lyrics});
            cur_byte += 4 + lyrics.size() + 1; // 4 = GetTime reads 4 bytes
        }
        
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        std::cout << "Language: " << language << '\n';
        std::cout << "Content descriptor: " << desc << '\n';
        for (const auto& i : time_data) {
            std::cout << i.first << ' ' << i.second << '\n';
        }
        std::cout << '\n';
    }

    char time_stamp_format;
    char content_type;
    std::vector<std::pair<uint32_t, std::string>> time_data;
};

/*
<Header for 'Commercial frame', ID: "COMR">
Text encoding      $xx
Price string       <text string> $00
Valid until        <text string> $00
Contact URL        <text string> $00
Received as        $xx
Name of seller     <text string according to encoding> $00 (00)
Description        <text string according to encoding> $00 (00)
Picture MIME type  <string> $00
Seller logo        <binary data>
*/

class COMRFrame: public Frame {
public:
    COMRFrame(std::ifstream& in, const std::string& file_) : Frame(in) {
        type = "COMR Frame";
        file = file_;
    }

private:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        price = ReadDataToZeroByte(in, encoding);
        valid_until = ReadDataToZeroByte(in, encoding);
        contact = ReadDataToZeroByte(in, encoding);
        recieved_as = in.get();
        seller = ReadDataToZeroByte(in, encoding);
        desc = ReadDataToZeroByte(in, encoding);
        MIME = ReadDataToZeroByte(in, encoding);

        size_t delim_pos = MIME.find('/');
        std::string type = delim_pos == std::string::npos ? ".undefined" : "." + MIME.substr(delim_pos + 1);
        std::ofstream logo(file + type, std::ios::binary);
        size_t bytes_left = size - ENCODING_SIZE - price.size() - 1 - valid_until.size() - 1 -
                                            contact.size() - 1 - 1 - seller.size() - 1 - desc.size() - 1 - MIME.size() - 1;
        for (size_t i = 0; i < bytes_left; ++i) {
            char byte = in.get();
            logo.write(&byte, 1);
        }
    }

    void Print(std::ostream& out) const override  {
        out << "This is: " << type << '\n';
        out << "Price: " << price << '\n';
        out << "Seller: " << seller << '\n';
        out << "Description: " << desc << '\n';
    }

    char encoding;
    char recieved_as;
    std::string price;
    std::string valid_until;
    std::string contact;
    std::string seller;
    std::string desc;
    std::string MIME;
    std::string file;
};

/*
<Header for 'Encryption method registration', ID: "ENCR">
Owner identifier    <text string> $00
Method symbol       $xx
Encryption data     <binary data>
*/

class ENCRFrame: public Frame {
public:
    ENCRFrame(std::ifstream& in) : Frame(in) {
        type = "ENCR Frame";
    }

private:
    void Read(std::ifstream& in) override {
        owner_id = ReadDataToZeroByte(in, 0x03);
        method = in.get();
        std::ofstream out("secret_data");
        for (size_t i = 0; i < size - owner_id.size() - 1 - 1; ++i) {
            char byte = in.get();
            out.write(&byte, 1);
        }
    }

    void Print(std::ostream& out) const override  {
        out << "Type: " << type << '\n';
        out << "Owner id: " << owner_id << '\n';
    }

    std::string owner_id;
    char method;    
};

/*
<Header of 'Equalisation (2)', ID: "EQU2">
Interpolation method  $xx
Identification        <text string> $00
Frequency          $xx xx
Volume adjustment  $xx xx
*/

class EQU2Frame: public Frame {
public:
    EQU2Frame(std::ifstream& in) : Frame(in) {
        type = "EQU2 Frame";
        freq = 0;
        volume = 0;
    }

private:
    void Read(std::ifstream& in) override {
        interpolation_method = in.get();
        id = ReadDataToZeroByte(in, 0x03);
        for (size_t i = 0; i < 2; ++i) {
            char byte = in.get();
            freq |= (unsigned char) byte << ((1 - i) * 8);
        }
        for (size_t i = 0; i < 2; ++i) {
            char byte = in.get();
            volume |= (unsigned char) byte << ((1 - i) * 8);
        }
    }

    void Print(std::ostream& out) const override  {
        std::cout << "Type: " << type << '\n';
        std::cout << "Interpolation method " << interpolation_method << '\n';
        std::cout << "Identification " << id << '\n';
        std::cout << "Frequency and volume " << freq << ' ' << volume << '\n';
        
    }

    char interpolation_method;
    std::string id;
    uint16_t freq;
    uint16_t volume;
};

/*
<Header for 'Linked information', ID: "LINK">
Frame identifier        $xx xx xx xx
URL                     <text string> $00
ID and additional data  <text string(s)>
*/

class LINKFrame: public Frame {
public:
    LINKFrame(std::ifstream& in) : Frame(in) {
        type = "LINK Frame";
        id.resize(FRAME_ID_SIZE);
    }

private:
    void Read(std::ifstream& in) override {
        std::cout << "Type: " << type << '\n';
        in.read(id.data(), id.size());
        url = ReadDataToZeroByte(in, 0x03);
        size_t cur_byte = id.size() + url.size() + 1;
        while (cur_byte < size) {
            data.push_back(ReadDataToZeroByte(in, 0x03));
            cur_byte += data.back().size();
        }
    }

    void Print(std::ostream& out) const override  {
        std::cout << "Type: " << type << '\n';
        std::cout << "ID: " << id << '\n';
        std::cout << "URL: " << url << '\n';
        std::cout << "Data: \n";
        for (const auto& i : data) {
            std::cout << i << '\n';
        }
        std::cout << '\n';
    }

    std::string id;
    std::string url;
    std::vector<std::string> data;
};

/*
<Header for 'Ownership frame', ID: "OWNE">
Text encoding     $xx
Price paid        <text string> $00
Date of purch.    <text string>
Seller            <text string according to encoding>
*/

class OWNEFrame: public Frame {
public:
    OWNEFrame(std::ifstream& in) : Frame(in) {
        type = "OWNE Frame";
        date.resize(DATE_SIZE);
    }

private:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        paid = ReadDataToZeroByte(in, encoding);
        in.read(date.data(), date.size());
        seller.resize(size - ENCODING_SIZE - paid.size() - 1 - date.size());
        ReadData(encoding, in, seller);
    }

    void Print(std::ostream& out) const override  {
         std::cout << "Type: " << type << '\n';
         std::cout << "Price paid: " << paid << '\n';
         std::cout << "Date <YYYYMMDD>: " << date << '\n';
         std::cout << "Seller: " << seller << '\n';
    }

    char encoding;
    std::string paid;
    std::string date;
    std::string seller;
};

/*
<Head for 'Position synchronisation', ID: "POSS">
Time stamp format         $xx
Position                  $xx (xx ...)
*/

class POSSFrame: public Frame {
public:
    POSSFrame(std::ifstream& in) : Frame(in) {
        type = "POSS Frame";
    }

private:
    void Read(std::ifstream& in) override {
        time_stamp_format = in.get();
        position = GetTime(in);
    }

    void Print(std::ostream& out) const override  {
         std::cout << "Type: " << type << '\n';
         std::cout << "Time stamp format: " << time_stamp_format << '\n';
         std::cout << "Position of something: " << position << '\n';
    }

    char time_stamp_format;
    uint32_t position;
};

/*
<Header for 'Recommended buffer size', ID: "RBUF">
Buffer size               $xx xx xx xx
Embedded info flag        %0000000x
Offset to next tag        $xx xx xx xx
*/

class RBUFFrame: public Frame {
public:
    RBUFFrame(std::ifstream& in) : Frame(in) {
        type = "RBUF Frame";
    }

private:
    void Read(std::ifstream& in) override {
        buffer_size = GetTime(in);
        char byte = in.get();
        embedded_info_flag = (bool)byte;
        offset = GetTime(in);
    }

    void Print(std::ostream& out) const override  {
         std::cout << "Type: " << type << '\n';
         std::cout << "Buffer size: " << buffer_size << '\n';
         std::cout << "Offset: " << offset << '\n';
    }

    uint32_t buffer_size;
    bool embedded_info_flag;
    size_t offset;
};

// <Header for 'Relative volume adjustment (2)', ID: "RVA2">
// Identification          <text string> $00

class RVA2Frame: public Frame {
public:
    RVA2Frame(std::ifstream& in) : Frame(in) {
        type = "RVA2 Frame";
        volume = 0;
    }

private:
    void Read(std::ifstream& in) override {
        channel_type = in.get();
        for (size_t i = 0; i < 2; ++i) {
            char byte = in.get();
            volume |= (unsigned char) byte << ((1 - i) * 8);
        }
        bits_representing_peak = in.get();
        peak_volume = GetTime(in);
    }

    void Print(std::ostream& out) const override  {
         std::cout << "Type: " << type << '\n';
         std::cout << "Channel type: " << ' ' << channel_type << '\n';
         std::cout << "Volume: " << volume << '\n';
         std::cout << "Peak volume: " << peak_volume << '\n';
    }

    char channel_type;
    uint16_t volume;
    char bits_representing_peak;
    uint32_t peak_volume;
};

/*
<Header for 'Seek frame', ID: "SEEK">
Minimum offset to next tag       $xx xx xx xx
*/

class SEEKFrame: public Frame {
public:
    SEEKFrame(std::ifstream& in) : Frame(in) {
        type = "SEEK Frame";
    }

private:
    void Read(std::ifstream& in) override {
        offset = GetTime(in);
    }

    void Print(std::ostream& out) const override  {
         std::cout << "Type: " << type << '\n';
         std::cout << "Offset: " << offset << '\n';
    }

    size_t offset;
};

/*
<Header for 'Unique file identifier', ID: "UFID">
Owner identifier        <text string> $00
Identifier              <up to 64 bytes binary data>
*/

class UFIDFrame: public Frame {
public:
    UFIDFrame(std::ifstream& in) : Frame(in) {
        type = "UFID Frame";
    }

private:
    void Read(std::ifstream& in) override {
        owner_id = ReadDataToZeroByte(in, 0x03);
        id.resize(size - owner_id.size() - 1);
        in.read(id.data(), id.size());
    }

    void Print(std::ostream& out) const override  {
         std::cout << "Type: " << type << '\n';
         std::cout << "Owner id: " << owner_id << '\n';

    }

    std::string owner_id;
    std::string id;
};

/*
<Header for 'Terms of use frame', ID: "USER">
Text encoding        $xx
Language             $xx xx xx
The actual text      <text string according to encoding>
*/

class USERFrame: public LanguageFrame {
public:
    USERFrame(std::ifstream& in) : LanguageFrame(in) {
        type = "USER Frame";
    }

private:
    void Read(std::ifstream& in) override {
        encoding = in.get();
        in.read(language.data(), language.size());
        data.resize(size - 1 - language.size());
        in.read(data.data(), data.size());
    }

    void Print(std::ostream& out) const override  {
         std::cout << "Type: " << type << '\n';
         std::cout << "Encoding: " << MapEncodingToText(encoding) << '\n';
         std::cout << "Language: " << language << '\n';
         std::cout << "Data: " << data << '\n';
    }
};
