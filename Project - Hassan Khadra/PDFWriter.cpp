#include <iostream>
#include "PDFWriter.hpp"

static const std::string OP_BEGIN_TEXT = "BT";
static const std::string OP_CONCAT_MATRIX = "cm";
static const std::string OP_DO = "Do";
static const std::string OP_END_TEXT = "ET";
static const std::string OP_FILL = "f";
static const std::string OP_LINETO = "l";
static const std::string OP_MOVETO = "m";
static const std::string OP_SAVE = "q";
static const std::string OP_RESTORE = "Q";
static const std::string OP_LINE_COLOR = "RG";
static const std::string OP_FILL_COLOR = "rg";
static const std::string OP_STROKE = "S";
static const std::string OP_SET_FONT = "Tf";
static const std::string OP_TEXT_SHOW = "Tj";
static const std::string OP_TEXT_MATRIX = "Tm";
static const std::string OP_RECTANGLE = "re";
static const std::string OP_WIDTH = "w";

const std::string pdfwrite::PDFWriter::FONTS[] =
    {
        "Courier",
        "Courier-Bold",
        "Courier-Oblique",
        "Courier-BoldOblique",
        "Helvetica",
        "Helvetica-Bold",
        "Helvetica-Oblique",
        "Helvetica-BoldOblique",
        "Symbol",
        "Times-Roman",
        "Times-Bold",
        "Times-Italic",
        "Times-BoldItalic",
        "ZapfDingbats"};

const int pdfwrite::PDFWriter::N_FONTS = sizeof(pdfwrite::PDFWriter::FONTS) / sizeof(pdfwrite::PDFWriter::FONTS[0]);

pdfwrite::PDFWriter::PDFWriter()
{
    pWidth = PDF_A4_WIDTH;
    pHeight = PDF_A4_HEIGHT;
}

pdfwrite::PDFWriter::PDFWriter(int width, int height)
{
    pWidth = width;
    pHeight = height;
}

pdfwrite::PDFWriter::~PDFWriter()
{
}

int pdfwrite::PDFWriter::pdfGetWidth() const
{
    return pWidth;
}

int pdfwrite::PDFWriter::pdfGetHeight() const
{
    return pHeight;
}

void pdfwrite::PDFWriter::pdfSetFont(Font font, int size)
{
    pdfmeta.font = font;
    pdfmeta.size = size;
}

void pdfwrite::PDFWriter::warpText(int maxWidth, bool isRightJustified)
{
    std::vector<std::string> words = getWordsVec(pdfmeta.content);
    std::vector<std::string> resultString = setResultantString(words, maxWidth);
    justifyString(resultString, isRightJustified, maxWidth);
    pdfmeta.resultString = resultString;
}

std::vector<std::string> pdfwrite::PDFWriter::getWordsVec(const std::string &text)
{

    std::vector<std::string> words;
    std::string currentWord;

    for (char ch : text)
    {
        if (currentWord != "")
        {
            if (isspace(ch))
            {
                words.push_back(currentWord);
                currentWord = "";
            }
            else
            {
                currentWord += ch;
            }
        }
        else
        {
            if (!isspace(ch))
            {
                currentWord += ch;
            }
        }
    }
    if (currentWord != "")
    {
        words.push_back(currentWord);
    }

    return words;
}

std::vector<std::string> pdfwrite::PDFWriter::setResultantString(const std::vector<std::string> &words, int maxWidth)
{
    std::vector<std::string> result;
    std::string currentLine;

    for (int i = 0, n = words.size(); i < n; i++)
    {
        std::string s = currentLine;

        if (currentLine != "")
        {
            switch (currentLine[currentLine.size() - 1])
            {
            case '.':
            case '?':
            case '!':
                s += " ";
                break;
            }

            s += " ";
        }

        s += words[i];

        if (stringWidth(s) > maxWidth)
        {
            result.push_back(currentLine);
            currentLine = words[i];
        }
        else
            currentLine = s;
    }

    result.push_back(currentLine);
    return result;
}

int pdfwrite::PDFWriter::stringWidth(Font currentFont, int currentSize, const std::string &text)
{
    int width = 0;

    int *metrics = pdfwrite::getFont(currentFont);

    if (metrics)
    {
        double dWidth = 0.0;

        for (int i = 0, n = text.size(); i < n; i++)
        {
            double value = metrics[text[i]] / 1000.0;
            dWidth += currentSize * value;
        }

        width = (int)(dWidth + 0.5);
    }

    return (width);
}

int pdfwrite::PDFWriter::stringWidth(const std::string &text)
{
    int width = 0;

    if (pdfmeta.font != NONE)
    {
        width = stringWidth(pdfmeta.font, pdfmeta.size, text);
    }

    return (width);
}

void pdfwrite::PDFWriter::justifyString(std::vector<std::string> &resultString, bool isRightJustified, int maxWidth)
{
    if (isRightJustified)
    {
        std::string aSpace = " ";
        int spaceWidth = stringWidth(aSpace);

        for (int i = 0, n = resultString.size(); i < n; i++)
        {
            std::string line = resultString[i];

            if (stringWidth(line) + spaceWidth <= maxWidth)
            {
                std::vector<int> indices;

                for (int j = 0; j < line.size(); j++)
                {
                    if (j > 0 && !isspace(line[j - 1]) && isspace(line[j]))
                        indices.push_back(j);
                }

                int nIndices = indices.size();

                if (nIndices > 0)
                {
                    int theIndex = 0;

                    while (stringWidth(line) + spaceWidth <= maxWidth)
                    {
                        line = line.substr(0, indices[theIndex]) +
                               aSpace +
                               line.substr(indices[theIndex]);

                        for (int j = theIndex + 1; j < nIndices; j++)
                            indices[j]++;

                        if (++theIndex == nIndices)
                            theIndex = 0;
                    }
                }

                resultString[i] = line;
            }
        }
    }
}

void pdfwrite::PDFWriter::pdfSetContent(std::string content)
{
    pdfmeta.content = content;
}

std::string pdfwrite::PDFWriter::pdfGetContent()
{
    return pdfmeta.content;
}

std::vector<std::string> pdfwrite::PDFWriter::pdfGetWrappedText()
{
    return pdfmeta.resultString;
}

void pdfwrite::PDFWriter::pdfAddToPage()
{
    pageMeta.emplace_back(pdfmeta);
}

pdfwrite::Font pdfwrite::PDFWriter::pdfGetFont()
{
    return pdfmeta.font;
}

void pdfwrite::PDFWriter::setWidthHeight(int width, int height)
{
    pWidth = width;
    pHeight = height;
}

void pdfwrite::PDFWriter::pdfAddNewPage()
{

    pdfmeta = PdfDocumentMeta();
    pdfmeta.content = "";
    pdfmeta.font = pdfwrite::Font::NONE;
    pdfmeta.resultString.clear();
    pdfmeta.size = 0;
    pdfmeta.x = 0;
    pdfmeta.y = 0;
}

std::string pdfwrite::PDFWriter::toString()
{
    std::vector<int> offsets;
    std::ostringstream out;

    out << "%PDF-1.1"
        << "\n\n";

    getStandardFonts(out, offsets);

    int imageIndexId = imageObjectId(0);

    offsets.push_back(out.str().size());

    out << imageIndexId << " 0 obj"
        << "\n"
        << "<<"
        << "\n"
        << "   /Font <<"
        << "\n";

    for (int i = 0; i < N_FONTS; i++)
    {
        out << "      /F" << (i + 1) << " " << (i + 1) << " 0 R"
            << "\n";
    }

    out << "   >>"
        << "\n";

    out << ">>"
        << "\n"
        << "endobj"
        << "\n\n";

    offsets.push_back(out.str().size());

    int idCatalog = (imageIndexId + 1);
    int idPages = (idCatalog + 1);

    out << idCatalog << " 0 obj"
        << "\n"
        << "<<"
        << "\n"
        << "   /Type /Catalog"
        << "\n"
        << "   /Pages " << idPages << " 0 R"
        << "\n"
        << ">>"
        << "\n"
        << "endobj"
        << "\n\n";

    offsets.push_back(out.str().size());

    setPageMeta(out, idPages);

    setPageContent(out, offsets, idPages, imageIndexId);

    int xrefOffset = out.str().size();
    int theSize = (offsets.size() + 1);

    std::string append = " ";

    out << "xref"
        << "\n"
        << "0 " << theSize << "\n"
        << "0000000000 65535 f" << append << "\n";

    for (int i = 0, n = offsets.size(); i < n; i++)
    {
        std::ostringstream tmp;

        tmp << offsets[i];

        for (int j = 0, nn = 10 - tmp.str().size(); j < nn; j++)
            out << "0";

        out << offsets[i] << " 00000 n" << append << "\n";
    }

    out << "trailer"
        << "\n"
        << "<<"
        << "\n"
        << "   /Size " << theSize << "\n"
        << "   /Root " << idCatalog << " 0 R"
        << "\n"
        << ">>"
        << "\n"
        << "startxref"
        << "\n"
        << xrefOffset << "\n"
                         "%%EOF\n";

    return out.str();
}

void pdfwrite::PDFWriter::getStandardFonts(std::ostringstream &out, std::vector<int> &offsets)
{
    for (int i = 0; i < N_FONTS; i++)
    {
        offsets.push_back(out.str().size());

        out << (1 + i) << " 0 obj"
            << "\n"
            << "<<"
            << "\n"
            << "   /Type     /Font"
            << "\n"
            << "   /Subtype  /Type1"
            << "\n"
            << "   /BaseFont /" << pdfwrite::PDFWriter::FONTS[i] << "\n"
            << "   /Encoding /WinAnsiEncoding"
            << "\n"
            << ">>"
            << "\n"
            << "endobj"
            << "\n\n";
    }
}

int pdfwrite::PDFWriter::imageObjectId(int index)
{
    return pdfwrite::PDFWriter::N_FONTS + index + 1;
}

void pdfwrite::PDFWriter::setPageMeta(std::ostringstream &out, int idPages)
{

    out << idPages << " 0 obj"
        << "\n"
        << "<<"
        << "\n"
        << "   /Type /Pages"
        << "\n"
        << "   /MediaBox ";

    out << "[ 0 0 " << pWidth << " " << pHeight << " ]"
        << "\n";

    out << "   /Count " << pageMeta.size() << "\n"
        << "   /Kids [";

    for (int i = 0; i < pageMeta.size(); i++)
    {
        out << " " << (1 + idPages + i) << " 0 R";
    }

    for (int i = 0; i < pageMeta.size(); i++)
        out << " " << (1 + idPages + i) << " 0 R";

    out << " ]"
        << "\n"
        << ">>"
        << "\n"
        << "endobj"
        << "\n\n";
}

void pdfwrite::PDFWriter::setPageContent(std::ostringstream &out, std::vector<int> &offsets, int idPages, int imageIndexId)
{
    for (int i = 0; i < pageMeta.size(); i++)
    {
        offsets.push_back(out.str().size());

        int idPage = (1 + idPages + i);

        out << idPage << " 0 obj"
            << "\n"
            << "<<"
            << "\n"
            << "   /Type /Page"
            << "\n"
            << "   /Parent " << idPages << " 0 R"
            << "\n"
            << "   /Contents " << (idPage + pageMeta.size()) << " 0 R"
            << "\n"
            << "   /Resources " << imageIndexId << " 0 R"
            << "\n"
            << ">>"
            << "\n"
            << "endobj"
            << "\n\n";
    }

    // Now our page contents

    int idPageContent = idPages + pageMeta.size() + 1;

    for (int i = 0; i < pageMeta.size(); i++)
    {
        std::string content = pageContentToString(pageMeta[i]);

        content = OP_BEGIN_TEXT + "\n" + content + OP_END_TEXT;
        int size = content.size();

        offsets.push_back(out.str().size());

        out << idPageContent << " 0 obj"
            << "\n"
            << "<<"
            << "\n"
            << "   /Length " << size << "\n"
            << ">>"
            << "\n"
            << "stream"
            << "\n"
            << content << "\n"
            << "endstream"
            << "\n"
            << "endobj"
            << "\n\n";

        idPageContent++;
    }
}

std::string pdfwrite::PDFWriter::pageContentToString(struct PdfDocumentMeta documentMeta)
{
    std::ostringstream contentString;

    getFontData(documentMeta, contentString);

    showText(documentMeta, contentString);

    return contentString.str();
}

void pdfwrite::PDFWriter::getFontData(struct PdfDocumentMeta &documentMeta, std::ostringstream &contentString)
{
    contentString << "/F" << (int)documentMeta.font << " " << documentMeta.size << " " << OP_SET_FONT << "\n";
    ;
}

void pdfwrite::PDFWriter::showText(struct PdfDocumentMeta &documentMeta, std::ostringstream &contentString)
{
    for (int i = 0; i < documentMeta.resultString.size(); i++)
    {
        std::string textOnPage;
        std::string str = documentMeta.resultString[i];
        for (int j = 0; j < str.size(); j++)
        {
            if (str[j] == '(' || str[j] == ')')
            {
                textOnPage += "\\";
            }
            textOnPage += str[j];
        }

        std::ostringstream text;

        text << "1 0 0 1"
             << " " << documentMeta.x << " " << documentMeta.y - 20 * i << " " << OP_TEXT_MATRIX << "\n";
        text << "(" << textOnPage << ")"
             << " " << OP_TEXT_SHOW << "\n";

        contentString << text.str();
    }
}

bool pdfwrite::PDFWriter::writeToFile(const std::string &fileName, std::string &errMsg)
{
    std::ofstream theFile(fileName.c_str(), std::ios::out | std::ios::binary);

    if (!theFile)
    {
        errMsg = "There is a problem opening the file: " + fileName;
        return (false);
    }

    std::string str = toString();

    theFile.write(str.c_str(), str.size());

    theFile.close();

    return (true);
}

void pdfwrite::PDFWriter::setXY(int x, int y)
{
    pdfmeta.x = x;
    pdfmeta.y = y;
}

int *pdfwrite::getFont(pdfwrite::Font theFont)
{
    int *metrics = nullptr;

    switch (theFont)
    {
    case pdfwrite::COURIER:
        metrics = courier;
        break;
    case pdfwrite::COURIER_BOLD:
        metrics = courier_bold;
        break;
    case pdfwrite::COURIER_OBLIQUE:
        metrics = courier_oblique;
        break;
    case pdfwrite::COURIER_BOLD_OBLIQUE:
        metrics = courier_boldoblique;
        break;
    case pdfwrite::HELVETICA:
        metrics = helvetica;
        break;
    case pdfwrite::HELVETICA_BOLD:
        metrics = helvetica_bold;
        break;
    case pdfwrite::HELVETICA_OBLIQUE:
        metrics = helvetica_oblique;
        break;
    case pdfwrite::HELVETICA_BOLD_OBLIQUE:
        metrics = helvetica_boldoblique;
        break;
    case pdfwrite::SYMBOL:
        metrics = symbol;
        break;
    case pdfwrite::TIMES:
        metrics = times_roman;
        break;
    case pdfwrite::TIMES_BOLD:
        metrics = times_bold;
        break;
    case pdfwrite::TIMES_ITALIC:
        metrics = times_italic;
        break;
    case pdfwrite::TIMES_BOLD_ITALIC:
        metrics = times_bolditalic;
        break;
    case pdfwrite::ZAPF_DINGBATS:
        metrics = zapfdingbats;
        break;

    case pdfwrite::NONE:
        break;
    }

    return (metrics);
}