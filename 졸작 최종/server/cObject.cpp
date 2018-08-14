#include "stdafx.h"
#include "cObject.h"
#include "cMyNamespace.h"
#include "cThreadFunc.h"

cObject::cObject() : is_use(false), id(-1), x((float)(BOARD_WIDTH/2)), z((float)(BOARD_WIDTH/2)), sight_x(0.0f), sight_z(1.0f)
{
	exover.command = E_RECV;
	scene = STAGE_ONE;
}
cObject::~cObject()
{
}

int cObject::GetAnimNum()
{
	if (m_pCurrentState == Ideal::Instance())
		return 0;
	/*else if (m_pCurrentState == Move::Instance())
		return 1;
	else if (m_pCurrentState == Attack::Instance())
		return 2;
	else if (m_pCurrentState == BeAttack::Instance())
		return 3;
	else if (m_pCurrentState == Rush::Instance())
		return 1;*/
	else
		return 0;
}


void cObject::UpdatesSectorSearch()
{
	RECT rc;
	rc.left = x - VIEW_RADIUS;
	rc.right = x + VIEW_RADIUS;
	rc.top = z - VIEW_RADIUS;
	rc.bottom = z + VIEW_RADIUS;

	AdjustRect(rc);

	near_sector.clear();
	near_list.clear();
	temp_obj.clear();
	
	/*InsertSector(near_sector, GetCurrZone()->GetSector(x, z));
	InsertSector(near_sector, GetCurrZone()->GetSector(rc.left, rc.top));
	InsertSector(near_sector, GetCurrZone()->GetSector(rc.right, rc.top));
	InsertSector(near_sector, GetCurrZone()->GetSector(rc.left, rc.bottom));
	InsertSector(near_sector, GetCurrZone()->GetSector(rc.right, rc.bottom));*/

	/*for (auto sector : near_sector)
	{
		sector->sector_mutex.lock();
		unordered_set<UINT> temp = sector->GetInPlayer();
		sector->sector_mutex.unlock();
	}*/
	for (int i = 0; i < MAX_OBJECT_INDEX; ++i) {
		if (objects[i]->is_use) {
			mtx.lock();
			temp_obj.insert(i);
			mtx.unlock();
		}
	}
	for (auto near_player : temp_obj)
	{
		if (near_player == id)
			continue;
		if (CanSee(near_player, id) == true)
		{
			mtx.lock();
			near_list.insert(near_player);
			mtx.unlock();
		}
	}
}
void cObject::AdjustRect(RECT& rc)
{
	if (rc.left < 0)
		rc.left = 0;
	if (rc.top < 0)
		rc.top = 0;
	if (rc.right >= BOARD_HEIGHT - 1)
		rc.right = BOARD_HEIGHT - 1;
	if (rc.bottom >= BOARD_HEIGHT - 1)
		rc.bottom = BOARD_HEIGHT - 1;
}
bool cObject::CanSee(const UINT i, const UINT j)
{
	int x = objects[i]->GetX() - objects[j]->GetX();
	int y = objects[i]->GetZ() - objects[j]->GetZ();

	int dist = x * x + y * y;

	return (dist <= (VIEW_RADIUS) * (VIEW_RADIUS));
}
void cObject::InsertSector(vector<cSector*>& near_sector, cSector* sector)
{
	auto iter = find(near_sector.begin(), near_sector.end(), sector);

	if (iter == near_sector.end())
		near_sector.push_back(sector);
}



/*============================*/
void cObject::ChangeState(BaseState *nowState)
{
	nowState->Exit(this);
	m_pPrevState = m_pCurrentState;
	m_pCurrentState = nowState;
	state_change = false;
	nowState->Enter(this);
}

void cObject::Move(float *temp_x, float *temp_y, float speed)
{
	*temp_x = *temp_x + sight_x * speed;
	*temp_y = *temp_y + sight_z * speed;
}
