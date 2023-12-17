#include <common.h>
#include <CVector.h>
#include <stdint.h>

enum {
	NUM_RUBBISH_SHEETS = 16
};

typedef struct COneSheet COneSheet;

struct COneSheet {
	CVector m_basePos;
	CVector m_animatedPos;
	float m_targetZ;
	uint8_t m_state;
	uint8_t m_animationType;
	uint32_t m_moveStart;
	uint32_t m_moveDuration;
	float m_animHeight;
	float m_xDist;
	float m_yDist;
	float m_angle;
	bool m_isVisible;
	bool m_targetIsVisible;
	COneSheet* m_next;
	COneSheet* m_prev;
};

void COneSheet_AddToList(COneSheet* this, COneSheet* list);
void COneSheet_RemoveFromList(COneSheet* this);


void CRubbish_Render();
void CRubbish_StirUp(CPhysical* veh);
void CRubbish_Update();
void CRubbish_SetVisibility(bool visible);
void CRubbish_Init();
void CRubbish_Shutdown();
