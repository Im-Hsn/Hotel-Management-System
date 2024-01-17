#ifndef PDFWRITER_HPP
#define PDFWRITER_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <cctype>
#include <fstream>
#include "PDFWriterStructs.hpp"

namespace pdfwrite
{
    /* 
        Class for pdf writing and creating a pdf document.
     */
    class PDFWriter
    {
    private:
        struct PdfDocumentMeta
        {
            pdfwrite::Font font;
            int size;
            std::string content;
            std::vector<std::string> resultString;
            int x;
            int y;
        } pdfmeta;

        int pWidth;
        int pHeight;

        std::vector<PdfDocumentMeta> pageMeta;

        /* 
            Method to split words and return the list of words.

            @param text
            @return vector<string> of splitted words.
         */
        std::vector<std::string> getWordsVec(const std::string &text);

        /* 
            Set the value to the resultant string.
            
            @param words
            @param maxwidth
            @return vector<string> of the string wrapped to max width based on font size.
         */
        std::vector<std::string> setResultantString(const std::vector<std::string> &words, int maxWidth);

        /* 
            Justify the contents of the string to right. And set the content according.

            @param resultString
            @param isRightJustified
         */
        void justifyString(std::vector<std::string> &resultString, bool isRightJustified, int maxWidth);

        /* 
            Method to find the string width of the font.
            
            @param text
            @return width of string of a perticular font.
         */
        int stringWidth(const std::string &text);

        /* 
            Method to get the string width of the font so that characters can be arranged according to
            page width.

            @param currentFont
            @param currentSize
            @param text
            @return width of string of a perticular font.
         */
        int stringWidth(Font currentFont, int currentSize, const std::string &text);

        /* 
            Method to convert the content of the page to string.

            @return string format of the pdf file.
         */
        std::string toString();

        /* 
            Method to get the standard fonts attached to string stream reference.

            @param out
         */
        void getStandardFonts(std::ostringstream &out, std::vector<int> &offsets);

        /* 
            Sotring images after fonts.

            @param index
            @return index to store image after fonts.
         */
        int imageObjectId(int index);

        /* 
            Method to set the page meta data.

            @param out
            @param idPages
         */
        void setPageMeta(std::ostringstream &out, int idPages);

        /* 
            Method to set the content in the pdf page.

            @param out
            @param offsets
            @param idPages
            @param imageIndexId
         */
        void setPageContent(std::ostringstream &out, std::vector<int> &offsets, int idPages, int imageIndexId);

        /* 
            Method to get the page data to string format.

            @param documentMeta
         */
        std::string pageContentToString(struct PdfDocumentMeta documentMeta);

        /* 
            Method to get the font data for pdf format.

            @param documentMeta
         */
        void getFontData(struct PdfDocumentMeta &documentMeta, std::ostringstream &contentString);

        /* 
            Method to get the text to be displated.

            @param documentMeta
         */
        void showText(struct PdfDocumentMeta &documentMeta, std::ostringstream &contentString);

    public:
        static const std::string FONTS[];
        static const int N_FONTS;

        /* 
            No args constructor for PDFWriter class.
            Set the default value of the page to PDF_A4_WIDTH, PDF_A4_HEIGHT.
         */
        PDFWriter();

        /* 
            Constructor to set the pdf page width and height and current font size to 0.

            @param width
            @param height
         */
        PDFWriter(int width, int height);

        /* 
            Method to set the page width and height.

            @param width
            @param height
         */
        void setWidthHeight(int width, int height);

        /* 
            Method to get the page width.

            @return width of the page.
         */
        int pdfGetWidth() const;

        /* 
            Method to get the page height.

            @return height of the page.
         */
        int pdfGetHeight() const;

        /* 
            Method to set the font and it's size.
            
            @param font
            @param size
         */
        void pdfSetFont(Font font, int size);

        /* 
            Method to get the font.
            
            @return the value of font in numeric value.
         */
        Font pdfGetFont();

        /* 
            Method to set the pdf pdf content text.

            @param content.
         */
        void pdfSetContent(std::string content);

        /* 
            Method to get the string content set.

            @return return the string of the content set.
         */
        std::string pdfGetContent();

        /* 
            Wrap text according to the page width and justify the content.
            
            @param maxWidth.
            @param isRightJustified.
         */
        void warpText(int maxWidth, bool isRightJustified);

        /* 
            Method to get the wrapped text

            @return vector<string> of the wrapped text.
         */
        std::vector<std::string> pdfGetWrappedText();

        /* 
            Add the values set of the page to vector<PdfDocumentMeta>.
         */
        void pdfAddToPage();

        /* 
            Method to add a new page.
         */
        void pdfAddNewPage();

        /* 
            Method to write content to file.

            @param fileName
            @param errMsg
         */
        bool writeToFile(const std::string &fileName, std::string &errMsg);

        /* 
            Method to set the X and Y axis in text. The text axis start from right for X
            and from bottom for Y.

            @param x
            @param y
         */
        void setXY(int x, int y);

        /* 
            Destructor for the PDFWriter class.
         */
        ~PDFWriter();
    };
} // namespace pdfwrite

#endif