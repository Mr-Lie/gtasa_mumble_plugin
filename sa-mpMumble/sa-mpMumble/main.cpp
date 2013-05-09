/* <your copyright here>
   Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com> 

   All rights reserved.
 
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met: 

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/ 

#include "mumble_plugin_win32.h"  

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

    // Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float pos_corrector[3];
	float front_corrector[3];
	float top_corrector[3];

	int player;
	peekProc((BYTE*)0xB6F5F0, &player, 4);

	if(player == 0)
		return true;
	bool ok;

	ok = peekProc((BYTE*)0xB6F9CC, &pos_corrector, 12)      &&

		 peekProc((BYTE*)0x0B6F9A0, &front_corrector[0], 4) &&
		 peekProc((BYTE*)0x0B6F9B0, &front_corrector[1], 4) &&
		 peekProc((BYTE*)0x0B6F9B4, &front_corrector[2], 4) &&

		 peekProc((BYTE*)0x0B6F9F4, &top_corrector[0], 4)   &&
		 peekProc((BYTE*)0x0B6F9C0, &top_corrector[1], 4)   &&
		 peekProc((BYTE*)0x0B6F9C4, &top_corrector[2], 4)   ;

	if(!ok) return false;

    // Convert to left-handed coordinate system

	avatar_pos[0] = pos_corrector[0];
	avatar_pos[1] = pos_corrector[2];
	avatar_pos[2] = pos_corrector[1];
	
	for (int i=0;i<3;i++)
		avatar_pos[i]/=32.0f; // Scale to meters

	avatar_front[0] = front_corrector[0];
	avatar_front[1] = front_corrector[2];
	avatar_front[2] = front_corrector[1];
	
	avatar_top[0] = top_corrector[0];
	avatar_top[1] = top_corrector[2];
	avatar_top[2] = top_corrector[1];
	
	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"gta_sa.exe"))
		return false;

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	std::wstring sidentity;
	std::string scontext;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports GTA:SA:MP.");
}

static std::wstring description(L"GTA:SA:MP");
static std::wstring shortname(L"GTA:SA:MP");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin aaplug = {
	MUMBLE_PLUGIN_MAGIC,
	description,
	shortname,
	NULL,
	NULL,
	trylock1,
	generic_unlock,
	longdesc,
	fetch
};

static MumblePlugin2 aaplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &aaplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &aaplug2;
}
