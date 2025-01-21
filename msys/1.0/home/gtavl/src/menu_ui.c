#include <stdio.h>
#include <stdint.h>
#include <injector.h>
#include <common.h>
#include <string.h>
#include <CFont.h>
#include <CRGBA.h>
#include <rwcore.h>
#include <CTxdStore.h>
#include <CTexture.h>
#include <CVector.h>

static RwRGBA windowBgColor, titleBgColor, textColor, titleColor, 
                selectTextBgColor, selectTextColor, grayTextColor, column2color;

static RwRGBA textureColor, emptyColor;

static CTexture menuBoughtSprite, menuSelectorSprite;
static CTexture pLogo, pPattern;

char* CText_Get(uint32_t *textData, const char *key);
extern uint32_t TheText;

void CSprite2d_DrawRect(CRect*, RwRGBA*);
void CSprite2d_DrawTextureRect(CTexture*, CRect*, RwRGBA*);
extern tMenuPanel *MenuInUse[2];

void CMessages_InsertNumberInString(char *, int, int, int, int, int, int, char *);

unsigned char CMenuSystem_CreateNewMenu(int type, char *pTitle, float posX, float posY, float width, char columns, bool interactive, bool background, int alignment);

void CMenuSystem_SwitchOffMenu(uint8_t);

typedef struct
{
    char m_nName[16];
    char m_nLogo[32], m_nPattern[32];
    int m_nInteriorID;
} HeaderInfo;

static HeaderInfo vecHeaders[24];
static int vecHeadersSize = 0;

#define LINE_BUFSIZE 256

static void MenuUi_ReadConfig()
{
    CRGBA_CRGBA(&windowBgColor, 0, 0, 0, 200);
    CRGBA_CRGBA(&titleBgColor, 0, 0, 0, 250);
    CRGBA_CRGBA(&textColor, 255, 255, 255, 255);
    CRGBA_CRGBA(&titleColor, 255, 255, 255, 255);
    CRGBA_CRGBA(&selectTextColor, 0, 0, 0, 255);
    CRGBA_CRGBA(&selectTextBgColor, 255, 255, 255, 255);
    CRGBA_CRGBA(&grayTextColor, 128, 128, 128, 255);
    CRGBA_CRGBA(&column2color, 255, 255, 255, 255);
    CRGBA_CRGBA(&textureColor, 255, 255, 255, 255);
    CRGBA_CRGBA(&emptyColor, 0, 0, 0, 0);
}

static void MenuUi_ReadHeaderInfo()
{
    RwFileFunction *fs = RwOsGetFileInterface();

    void* file = fs->rwfopen("DATA/HEADERS.DAT", "r");    
    char line[256];

    memset(vecHeaders, 0, sizeof(HeaderInfo)*25);

    while (fs->rwfgets(line, LINE_BUFSIZE, file))
    {
        if (line[0] == '#')
        {
            continue;
        }

        if (sscanf(line, "%s %s %s %d", vecHeaders[vecHeadersSize].m_nName, vecHeaders[vecHeadersSize].m_nLogo, vecHeaders[vecHeadersSize].m_nPattern, &vecHeaders[vecHeadersSize].m_nInteriorID) == 4)
        {
            vecHeadersSize++;
        } 
    }
}

/* 
    Printing text at the center pos with wrapping
*/
static void MenuUi_WrapXCenteredPrint(char* pGXT, CRect windowRect)
{
    char* pText = CText_Get(&TheText, pGXT);
    float textPosX = windowRect.left + (windowRect.right - windowRect.left) / 2;
    float textPosY = windowRect.top + (windowRect.bottom - windowRect.top) / 2 - 20.0f;
    float windowWidth = windowRect.right - windowRect.left;

    float fontWidth = CFont_GetStringWidth(pText, true, false);

    if (fontWidth <= windowWidth)
    {
        CFont_PrintString(textPosX, textPosY, pText);
        return;
    }
    char buf[256]; // Adjust the size accordingly
    char temp[256]; // Adjust the size accordingly
    memset(buf, 0, sizeof(buf));
    memset(temp, 0, sizeof(temp));

    char* token = strtok(pText, " ");
    while (token != NULL)
    {
        if (temp[0] == '\0')
        {
            strcpy(temp, token);
        }
        else
        {
            strcat(temp, " ");
            strcat(temp, token);
        }

        fontWidth = CFont_GetStringWidth(temp, true, false);

        if (fontWidth < windowWidth && token[0] != '\0')
        {
            strcat(buf, " ");
            strcat(buf, token);
        }
        else
        {
            strcpy(temp, token);
            CFont_PrintString(textPosX, textPosY, buf);
            CRect rect;
            CFont_GetTextRect(&rect, textPosX, textPosY, buf);
            textPosY -= (rect.bottom - rect.top) / 1.5f;
            strcpy(buf, token);
        }

        token = strtok(NULL, " ");
    }
}

static unsigned char MenuUi_CreateNewMenu(int type, char *pTitle, float posX, float posY, float width, char columns, bool interactive, bool background, int alignment) {
    //CStreaming_MakeSpaceFor(306464);
    //CStreaming_ImGonnaUseStreamingMemory();
    //CGame_TidyUpMemory(false, true);

    CTxdStore_PushCurrentTxd();
    int v7 = CTxdStore_FindTxdSlot("vlhud");
    if ( v7 == -1 )
        v7 = CTxdStore_AddTxdSlot("VLHUD");
    CTxdStore_LoadTxd(v7, "MODELS\\VLHUD.TXD");
    CTxdStore_AddRef(v7);
    CTxdStore_SetCurrentTxd(v7);

    CSprite2d_SetTexture(&menuBoughtSprite, "menu_bought", NULL);
    CSprite2d_SetTexture(&menuSelectorSprite, "menu_selector", NULL);

    int i;
    for (i = 0; i < vecHeadersSize; i++)
    {
        if (!strcmp(vecHeaders[i].m_nName, pTitle)/*
        && (*CGame_currArea == vecHeaders[i].m_nInteriorID || vecHeaders[i].m_nInteriorID == -1)*/ )
        {
            CSprite2d_SetTexture(&pLogo, vecHeaders[i].m_nLogo, 0);
            CSprite2d_SetTexture(&pPattern, vecHeaders[i].m_nPattern, 0);
            break; 
        }
    }

    CTxdStore_PopCurrentTxd();
    CTxdStore_RemoveTxd(v7);
    //CTxdStore_RemoveTxdSlot(v7);
    
    //CStreaming_IHaveUsedStreamingMemory();

    return CMenuSystem_CreateNewMenu(type, pTitle, posX, posY, width, columns, interactive, background, alignment);
}

static void MenuUi_SwitchOffMenu(unsigned char handle) {
    CSprite2d_Delete(&menuBoughtSprite);
    CSprite2d_Delete(&menuSelectorSprite);

    if (pLogo.texture) {
        CSprite2d_Delete(&pLogo);
        pLogo.texture = NULL;

    }
        
    if (pPattern.texture) {
        CSprite2d_Delete(&pPattern);
        pPattern.texture = NULL;
    }

    CMenuSystem_SwitchOffMenu(handle);
}

static void MenuUi_DisplayStandardMenu(unsigned char panelId, bool bBrightFont)
{
    tMenuPanel* pMenuPanel = MenuInUse[panelId];
    float textPaddingX = 640.0f / 64.0f;
    CVector2D menuPos = { textPaddingX, 0.0f };  
    float hBoxHeight = 448.0f / 30.0f ;
    float menuWidth = 640.0f / 3.5f;
    float headerPadding = 52.0f;

    // ---------------------------------------------------
    // font stuff 
    CVector2D fontScale = { 640.0f * 0.00045f , 448.0f * 0.0015f };
    CVector2D textScale = { fontScale.x, fontScale.y };
    CVector2D titleScale = { fontScale.x*1.25f, fontScale.y*1.25f };

    CFont_SetFontStyle(FONT_SUBTITLES);
    CFont_SetColor(&titleColor);
    CFont_SetOrientation(ALIGN_LEFT);
    CFont_SetDropColor(&emptyColor);

    // ---------------------------------------------------
    // Draw background
    if (pMenuPanel->m_bColumnBackground)
    {
        // ---------------------------------------------------
        // Draw title section


        CRect headerRect;
        headerRect.top = pMenuPanel->m_vPosn.y;
        headerRect.left = pMenuPanel->m_vPosn.x;
        headerRect.right = menuWidth + headerRect.left;
        menuPos.x += headerRect.left;
        menuPos.y = headerRect.top;

        headerRect.bottom = headerRect.top + headerPadding + hBoxHeight;
        
        // draw title sprites if available or go with text
        if (pPattern.texture)
        {
            CSprite2d_DrawTextureRect(&pPattern, &headerRect, &textureColor);
        }
        else
        {
            CSprite2d_DrawRect(&headerRect, &titleBgColor);
        }

        if (pLogo.texture)
        {
            CRect logoRect = headerRect;
            bool headers = false;

            // check if the panel got header text
            uint8_t column;
            for (column = 0; column < pMenuPanel->m_nNumColumns; ++column)
            {
                char* pText = CText_Get(&TheText, pMenuPanel->m_aacColumnHeaders[column]);

                if (pText[0] != ' ' && pText[0] != '\0')
                {
                    headers = true;
                    break;
                }
            }

            if (headers)
            {
                logoRect.bottom -= hBoxHeight;
            }

            CSprite2d_DrawTextureRect(&pLogo, &logoRect, &textureColor);
        }
        else
        {
            CFont_SetOrientation(ALIGN_CENTER);
            CFont_SetFontStyle(FONT_SUBTITLES);
            CFont_SetScale(titleScale.x, titleScale.y);

            MenuUi_WrapXCenteredPrint(pMenuPanel->m_acTitle, headerRect);

            CFont_SetFontStyle(FONT_SUBTITLES);
            CFont_SetOrientation(ALIGN_LEFT);
        }
        
        // ---------------------------------------------------
        // Draw text section
        CRect textRect;
        textRect.top = headerRect.bottom;
        textRect.left = headerRect.left;
        textRect.right = headerRect.right;
        textRect.bottom = textRect.top + (pMenuPanel->m_nNumRows * hBoxHeight); 
        
        CSprite2d_DrawRect(&textRect, &windowBgColor);

        // ---------------------------------------------------
        // Draw footer section

        CRect footerRect;
        footerRect.top = textRect.bottom;
        footerRect.bottom = footerRect.top + hBoxHeight;
        footerRect.left = textRect.left;
        footerRect.right = textRect.right;
        CSprite2d_DrawRect(&footerRect, &titleBgColor);

        // Draw selection sprite
        float posX = footerRect.left + (footerRect.right - footerRect.left - hBoxHeight)/2;
        CSprite2d_DrawSprite(&menuSelectorSprite, posX, footerRect.top, hBoxHeight, hBoxHeight, &textColor);

        // ---------------------------------------------------
        // Draw selection box
        CRect selectRect;
        selectRect.left = textRect.left;
        selectRect.right = textRect.right;
        selectRect.top = (headerRect.bottom + hBoxHeight * pMenuPanel->m_nSelectedRow);

        selectRect.bottom = selectRect.top + hBoxHeight;
        CSprite2d_DrawRect(&selectRect, &selectTextBgColor);

        // ---------------------------------------------------
    }

    uint8_t column;
    for (column = 0; column < pMenuPanel->m_nNumColumns; ++column)
    {
        CVector2D textPos = { menuPos.x + (column ? pMenuPanel->m_afColumnWidth[column - 1] : 0), menuPos.y + headerPadding};
        float scaleX = textScale.x;
        float fontWidth = 0.0f;

        char* pHeader = CText_Get(&TheText, pMenuPanel->m_aacColumnHeaders[column]);

        fontWidth = CFont_GetStringWidth(pHeader, true, false);
        if (fontWidth > menuWidth)
        {
            scaleX = scaleX - 1 + menuWidth / fontWidth;
        }

        CFont_SetOrientation(ALIGN_LEFT);
        CFont_SetColor(&titleColor);
        CFont_SetScale(scaleX, textScale.y);
        CFont_PrintString(textPos.x, textPos.y, pHeader);
        textPos.y += hBoxHeight;

        CFont_SetWrapx(pMenuPanel->m_afColumnWidth[column]);
        CFont_SetOrientation(pMenuPanel->m_anColumnAlignment[column]);


        // ---------------------------------------------------
        // Draw texts
        uint8_t row;
        for (row = 0; row < pMenuPanel->m_nNumRows; ++row)
        {
            char pText[400];
            int num = pMenuPanel->m_aadwNumberInRowTitle[column][row];
            int num2 = pMenuPanel->m_aadw2ndNumberInRowTitle[column][row];
            char* row_text = CText_Get(&TheText, pMenuPanel->m_aaacRowTitles[column][row]);
            CMessages_InsertNumberInString(row_text, num, num2, -1, -1, -1, -1, pText);
            //CMessages_InsertPlayerControlKeysInString(pText);

            scaleX = textScale.x;
            fontWidth = CFont_GetStringWidth(pText, true, false);
            if (fontWidth > menuWidth)
            {
                scaleX = scaleX - 1 + menuWidth / fontWidth;
            }

            RwRGBA color;
            if (column == 0) // first column
            {
                color = textColor;
            }
            else // 2nd column
            {
                color = column2color;
            }

            if (pMenuPanel->m_abRowSelectable[row])
            {
                if (row == pMenuPanel->m_nSelectedRow)
                {
                    color = selectTextColor;
                }
            }
            else
            {
                // Draw checkmark
                if (column == pMenuPanel->m_nNumColumns-1)
                {
                    float posX = textPos.x + fontWidth;
                    float posY = textPos.y;

                    posX += (1.0f);
                    posY -= (1.0f);

                    CSprite2d_DrawSprite(&menuBoughtSprite, posX, posY, hBoxHeight, hBoxHeight, &grayTextColor);
                }
                color = grayTextColor;
            }
            CFont_SetColor(&color);
            CFont_SetScale(scaleX, textScale.y);

            if (pMenuPanel->m_anColumnAlignment[column] == ALIGN_CENTER) {
                float columnWidth = 0.0f;
                uint8_t i;
                for (i = 0; i < pMenuPanel->m_nNumColumns-1; i++) {
                    columnWidth += pMenuPanel->m_afColumnWidth[i];
                }
                CFont_PrintString((textPos.x + columnWidth + (pMenuPanel->m_afColumnWidth[column]/2)) - textPaddingX, textPos.y, pText);
            } else if (pMenuPanel->m_anColumnAlignment[column] == ALIGN_RIGHT) {
                float columnWidth = 0.0f;
                uint8_t i;
                for (i = 0; i < pMenuPanel->m_nNumColumns; i++) {
                    columnWidth += pMenuPanel->m_afColumnWidth[i];
                }
                CFont_PrintString((pMenuPanel->m_vPosn.x + columnWidth - textPaddingX), textPos.y, pText); 
            } else {
                CFont_PrintString(textPos.x, textPos.y, pText); 
            }

            textPos.y += hBoxHeight;
        }

        // ---------------------------------------------------
    }
}

void setupInGameMenuPatches()
{
    MenuUi_ReadConfig();
    MenuUi_ReadHeaderInfo();

    RedirectCall(0x52CDD0, &MenuUi_DisplayStandardMenu);
    RedirectCall(0x5247F0, &MenuUi_CreateNewMenu);
    RedirectCall(0x5248FC, &MenuUi_SwitchOffMenu);
}
