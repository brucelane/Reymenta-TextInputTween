/*

Basic Spout sender for Cinder

Search for "SPOUT" to see what is required
Uses the Spout dll

Based on the RotatingBox CINDER example without much modification
Nothing fancy about this, just the basics.

Search for "SPOUT" to see what is required

==========================================================================
Copyright (C) 2014 Lynn Jarvis.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
==========================================================================

*/

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "cinder/Utilities.h"
#include "cinder/Timeline.h"
#include <vector>
#include <list>

#include "Resources.h"
#include "Character.h"
// spout
#include "spout.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ReymentaTextInputTweenApp : public AppNative {
public:
	void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();
	void shutdown();
	void keyDown(KeyEvent event);
	void mouseDown(MouseEvent event);

	void addChar(char c);
	void removeChar();

protected:
	Vec3f getRandomVec3f(float min, float max);

	gl::TextureFontRef	mTextureFont;

	vector<Character>	mCharacters;
	list<Character>		mDyingCharacters;
	string				mText;

	CameraPersp			mCam;
	float				mCamDist;

	Anim<Matrix44f>		mSceneMatrix;
	Matrix44f			mSceneDestMatrix;
private:
	// -------- SPOUT -------------
	SpoutSender spoutsender;                    // Create a Spout sender object
	bool bInitialized;                          // true if a sender initializes OK
	bool bMemoryMode;                           // tells us if texture share compatible
	unsigned int g_Width, g_Height;             // size of the texture being sent out
	char SenderName[256];                       // sender name 
	gl::Texture spoutTexture;                   // Local Cinder texture used for sharing
	// ----------------------------
	string currentText;
	vector<string>	strings;
	int stringIndex;
	int currentFrame;
	int frame;
	int startFrame;
	bool running;
};
