#include "parser.hpp"

bool IsBitSet(char chr, size_t bit) {
    return ((chr >> bit) & 1) == 1;
}

Header ReadHeader(std::ifstream& in) {
    Header header;
    in.read(header.file_id.data(), HEADER_FILE_ID_SIZE);
    if (header.file_id != "ID3") {
        std::cout << header.file_id << '\n';    
        std::cerr << "INCORRECT FILE\n";
        exit(1);
    }

    in.read(header.version.data(), HEADER_VERSION_SIZE); // should be 0x0400

    char flags;
    in.read(&flags, HEADER_FLAGS_SIZE);
    if (IsBitSet(flags, 7)) header.unsync = true;
    if (IsBitSet(flags, 6)) header.ext_header = true;
    if (IsBitSet(flags, 5)) header.exp_ind = true;
    if (IsBitSet(flags, 4)) header.footer = true;
    header.size = ReadSize(in);

    return header;
}

ExtHeader ReadExtHeader(std::ifstream& in) {
    ExtHeader ext_header;
    ext_header.size = ReadSize(in);
    in.read(&ext_header.num_of_flag_bytes, 1);
    in.read(&ext_header.flags, 1);

    if (IsBitSet(ext_header.flags, 7)) ext_header.restriction = true;
    if (IsBitSet(ext_header.flags, 6)) ext_header.crc = true;
    if (IsBitSet(ext_header.flags, 5)) ext_header.tag_is_upd = true;

    if (ext_header.crc) {
        in.read(&ext_header.flag_data_length, 1);
        std::string s_total_frame_crc(5, ' ');
        in.read(s_total_frame_crc.data(), s_total_frame_crc.size());
        ext_header.total_frame_crc |= s_total_frame_crc[0] << 28 
                                   | s_total_frame_crc[1] << 21 
                                   | s_total_frame_crc[2] << 14 
                                   | s_total_frame_crc[3] << 7 
                                   | s_total_frame_crc[4] << 0;
    }

    if (ext_header.restriction) {
        char flag_data_len;
        in.read(&flag_data_len, 1);
        char restrictions;
        in.read(&restrictions, 1);
        if (IsBitSet(restrictions, 7) && IsBitSet(restrictions, 6)) {
            ext_header.tag_size_restriction = 0; // No more than 128 frames and 1 MB total tag size.
        } else if (!IsBitSet(restrictions, 7) && IsBitSet(restrictions, 6)) {
            ext_header.tag_size_restriction = 1; // No more than 64 frames and 128 KB total tag size.
        } else if (IsBitSet(restrictions, 7) && !IsBitSet(restrictions, 6)) {
            ext_header.tag_size_restriction = 2; // No more than 32 frames and 40 KB total tag size.
        } else {
            ext_header.tag_size_restriction = 3; // No more than 32 frames and 4 KB total tag size.
        }

        if (IsBitSet(restrictions, 5)) {
            ext_header.text_enc_restriction = 1; // Strings are only encoded with ISO-8859-1 [ISO-8859-1] or UTF-8 [UTF-8].
        } else {
            ext_header.text_enc_restriction = 0; // No restrictionictions
        }

        if (IsBitSet(restrictions, 4) && IsBitSet(restrictions, 3)) {
            ext_header.text_field_size_restriction = 0; // No restrictionictions
        } else if (!IsBitSet(restrictions, 4) && IsBitSet(restrictions, 3)) {
            ext_header.text_field_size_restriction = 1; // No string is longer than 1024 characters.
        } else if (IsBitSet(restrictions, 4) && !IsBitSet(restrictions, 3)) {
            ext_header.text_field_size_restriction = 2; // No string is longer than 128 characters.
        } else {
            ext_header.text_field_size_restriction = 3; // No string is longer than 30 characters.
        }

        if (IsBitSet(restrictions, 2)) {
            ext_header.img_enc_restriction = 1; // Images are encoded only with PNG [PNG] or JPEG [JFIF].
        } else {
            ext_header.img_enc_restriction = 0; // No restrictionictions
        }

        if (!IsBitSet(restrictions, 1) && !IsBitSet(restrictions, 0)) {
            ext_header.img_size_restriction = 0; // No restrictionictions 
        } else if (!IsBitSet(restrictions, 1) && IsBitSet(restrictions, 0)) {
            ext_header.img_size_restriction = 1; // All images are 256x256 pixels or smaller.
        } else if (IsBitSet(restrictions, 1) && !IsBitSet(restrictions, 0)) {
            ext_header.img_size_restriction = 2; // All images are 64x64 pixels or smaller.
        } else {
            ext_header.img_size_restriction = 3; // All images are exactly 64x64 pixels, unless required otherwise.
        }
    }

    return ext_header;
}

size_t ReadSize(std::ifstream& in) {
    std::string size(4, ' ');
    in.read(size.data(), size.size());

    return size[0] << 21 | size[1] << 14 | size[2] << 7 | size[3];
}

std::string ReadDataToZeroByte(std::ifstream& in, size_t encoding) {
    std::string data;
    char byte;
    while (in.read(&byte, 1)) {
        if (byte == 0x00) break;
        data += byte;
    }

    if (encoding == 0x00) {
        return ISO_8859_TO_UTF_8(data);
    }

    return data;
}

std::ostream& operator<<(std::ostream& out, const Frame& frame) {
    frame.Print(out);
    return out;
}

std::ifstream& operator>>(std::ifstream& in, Frame& frame) {
    frame.Read(in);
    return in;
}

void Parse(const std::string& file) {
    std::ifstream in("../" + file, (std::ios::binary)|(std::ios::in));

    if (!in.is_open()) {
        std::cerr << "No such file to open\n";
        exit(EXIT_FAILURE);
    }

    Header header = ReadHeader(in);
    ExtHeader ext_header;

    if (header.ext_header)
        ext_header = ReadExtHeader(in);


    Frame* frame;
    size_t cur_byte = 0;
    size_t padding_size = 0;
    while (cur_byte < header.size) {
        std::string frame_id(4, ' ');
        in.read(frame_id.data(), 4);

        if (frame_id[0] == 0x00) {  // checking for padding
        padding_size += 4; // cuz i already read 4 bytes;
            char byte;
            while (in.read(&byte, 1)) {
                if (byte != 0x00) break;
                padding_size++;
            }

            // because i read 1 byte after footer, that can be important to smth
            in.seekg(-1, in.cur);
            break;
        }

        if (frame_id == "TXXX") {
            frame = new TXXXFrame(in);
        } else if (frame_id[0] == 'T') {
            frame = new TextFrame(in);
        } else if (frame_id == "COMM") {
            frame = new CommentFrame(in);
        } else if (frame_id == "POPM") {
            frame = new PopularimeterFrame(in);
        } else  if (frame_id == "USLT") {
            frame = new TranscriptionFrame(in);
        } else if (frame_id == "WXXX") {
            frame = new WXXXrame(in);
        } else if (frame_id[0] == 'W') {
            frame = new URLFrame(in);
        } else if (frame_id == "APIC") {
            frame = new PictureFrame(in, file);
        } else if (frame_id == "PCNT") {
            frame = new PlayCounterFrame(in);
        } else if (frame_id == "PRIV") {  // privet
            frame = new PrivateFrame(in);
        } else if (frame_id == "GRID") {
            frame = new GroupIdFrame(in);
        } else if (frame_id == "ETCO") {
            frame = new ETCOFrame(in);
        } else if (frame_id == "SYLT") {
            frame = new SYLTFrame(in);
        } else if (frame_id == "COMR") {
            frame = new COMRFrame(in, file);
        } else if (frame_id == "ENCR") {
            frame = new ENCRFrame(in);
        } else if (frame_id == "EQU2") {
            frame = new EQU2Frame(in);    
        } else if (frame_id == "LINK") {
            frame = new LINKFrame(in);    
        } else if (frame_id == "OWNE") {
            frame = new OWNEFrame(in);
        } else if (frame_id == "POSS") {
            frame = new POSSFrame(in);
        } else if (frame_id == "RBUF") {
            frame = new RBUFFrame(in);
        } else if (frame_id == "RVA2") {
            frame = new RVA2Frame(in);
        } else if (frame_id == "SEEK") {
            frame = new SEEKFrame(in);
        } else if (frame_id == "UFID") {
            frame = new UFIDFrame(in);
        } else if (frame_id == "USER") {
            frame = new USERFrame(in);
        } else {
            std::cout << "I don't know \"" << frame_id << "\" frame\n";
            break;
        }

        in >> *frame;
        std::cout << *frame;
        cur_byte += frame->Size();
        delete frame;
    }

    std::cout << cur_byte  << " + " << padding_size << " == " << header.size << '\n'; // cur_byte + padding_Size == header.size


    // check wheter is there footer
    in.seekg(-10, std::ios::end);
    std::string footer_id(3, ' ');
    in.read(footer_id.data(), 3);
    if (footer_id == "3DI") {
        std::cout << "Here is footer\n";
    }
}  

void ReadData(char encoding, std::ifstream& in, std::string& data) {
    size_t size = data.size();

    if (encoding == 0x01 || encoding == 0x02) { // utf-16
        if (encoding == 0x01) { // with bom
            in.seekg(2, std::ios::cur); // skip BOM
            size -= 2;
        }

        std::u16string u16(size / 2 + 1, '\0');
        in.read((char*)&u16[0], size);
        data = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16);   	
    } else {
        in.read(data.data(), size);
    }    

    if (encoding == 0x00) { // ISO
        data =  ISO_8859_TO_UTF_8(data);
    }
}



std::string ISO_8859_TO_UTF_8(const std::string &str) {
    std::string res;
    for (const unsigned char i : str) {
        if (i < 0x80) {
            res.push_back(i);
        } else {
            res.push_back(0xc0 | i >> 6);
            res.push_back(0x80 | (i & 0x3f));
        }
    }
    return res;
}

uint32_t GetTime(std::ifstream& in) {
    char byte;
    uint32_t time = 0;
    for (int i = 0; i < 4; ++i) {
        byte = in.get();
        time |= (unsigned char)byte << ((3 - i) * 8);
    }

    return time;
}

