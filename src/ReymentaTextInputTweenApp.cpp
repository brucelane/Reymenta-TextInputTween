#include "ReymentaTextInputTweenApp.h"

// -------- SPOUT -------------
void ReymentaTextInputTweenApp::prepareSettings(Settings *settings)
{
        g_Width  = 640;
        g_Height = 481;
        settings->setWindowSize( g_Width, g_Height );
        settings->setFullScreen( false );
        settings->setResizable( false ); // keep the screen size constant for a sender
        settings->setFrameRate( 60.0f );
}
// ----------------------------

void ReymentaTextInputTweenApp::setup()
{
    glEnable( GL_TEXTURE_2D );
    gl::enableDepthRead();
    gl::enableDepthWrite(); 

    // -------- SPOUT -------------
    // Set up the texture we will use to send out
    // We grab the screen so it has to be the same size
    spoutTexture =  gl::Texture(g_Width, g_Height);
    strcpy_s(SenderName, "Reymenta TextInputTween Sender"); // we have to set a sender name first
    // Optionally test for texture share compatibility
    // bMemoryMode informs us whether Spout initialized for texture share or memory share
    bMemoryMode = spoutsender.GetMemoryShareMode();
    // Initialize a sender
    bInitialized = spoutsender.CreateSender(SenderName, g_Width, g_Height);
    // TextInputTween
	gl::enableAlphaBlending();

	// font
	Font customFont(Font(loadResource(RES_TEXTTWEEN), 200));
	gl::TextureFont::Format f;
	f.enableMipmapping(true);
	mTextureFont = gl::TextureFont::create(customFont, f);

	// camera
	mCamDist = 600.0f;
	mCam.setPerspective(75.0f, getWindowAspectRatio(), 0.1f, 15000.0f);
	mCam.lookAt(Vec3f(0.0f, 0.0f, mCamDist), Vec3f::zero(), Vec3f::yAxis());
	mCam.setLensShiftHorizontal(-0.4);
	// scene
	mSceneMatrix = mSceneDestMatrix = Matrix44f::identity();

	// init text
	/*addChar('A'); addChar('N'); addChar('D'); addChar(' ');
	addChar('H');	addChar('E'); addChar(' ');
	addChar('S');	addChar('A'); addChar('Y'); addChar('S');*/
	running = false;
	stringIndex = 0;
	strings.push_back(" AND HE SAYS ");
	strings.push_back(" LIQUID LOVE, NO TIME NO TIME ABOVE ");
	strings.push_back(" ON THE 26, BABY WE'LL FIND BLISS ");
	currentFrame = -1;
	startFrame = 0;
	currentText = loadString(loadAsset("lyrics.txt"));
	setFrameRate(32.0f);
}
void ReymentaTextInputTweenApp::keyDown(KeyEvent event)
{
	char ch = event.getChar();
	string specialChars = " .,:;/?\\!@#%^&*()-_=+[]<>'\""; // the remaining characters that the font provides

	if (event.getCode() == KeyEvent::KEY_BACKSPACE)
		removeChar();
	else if (isalnum(ch))
		addChar(ch);
	else if (specialChars.find(ch) != -1)
		addChar(ch);
}
void ReymentaTextInputTweenApp::addChar(char c)
{
	if (c == 32) {
		c = 97;
	}
	else { 
		c = toupper(c);
	}
	int count = mCharacters.size();

	if (count > 0)
		mSceneDestMatrix.translate(Vec3f(mCharacters.back().getKernBounds().getWidth() / 2.0f, 0.0f, 0.0f));

	Matrix44f randStartMatrix = mSceneDestMatrix;
	randStartMatrix.translate(getRandomVec3f(100.0f, 600.0f));
	//randStartMatrix.rotate(getRandomVec3f(2.0f, 6.0f));

	mCharacters.push_back(Character(mTextureFont, string(&c, 1), randStartMatrix));

	mSceneDestMatrix.translate(Vec3f(mCharacters.back().getKernBounds().getWidth() / 2.0f, 0.0f, 0.0f));

	float t = (count + 281) / 50.0f;
	//mSceneDestMatrix.rotate(Vec3f(sin(t)*0.1f, cos(t)*0.2f, cos(t)*0.05f)); // makes the scene meander

	mCharacters.back().animIn(timeline(), mSceneDestMatrix);

	timeline().apply(&mSceneMatrix, mSceneDestMatrix, 1.0f, EaseOutAtan(10));
}

void ReymentaTextInputTweenApp::removeChar()
{
	if (!mCharacters.empty()) {
		mDyingCharacters.push_back(mCharacters.back());
		mCharacters.pop_back();

		if (!mCharacters.empty())
			mSceneDestMatrix = mCharacters.back().getDestMatrix();
		else
			mSceneDestMatrix = Matrix44f::identity();

		Matrix44f randStartMatrix = mSceneDestMatrix;
		randStartMatrix.translate(getRandomVec3f(100.0f, 600.0f));
		//randStartMatrix.rotate(getRandomVec3f(2.0f, 6.0f));

		mDyingCharacters.back().animOut(timeline(), randStartMatrix);

		timeline().apply(&mSceneMatrix, mSceneDestMatrix, 1.0f, EaseOutAtan(10));
	}
}
void ReymentaTextInputTweenApp::mouseDown(MouseEvent event)
{
	running = !running;
	startFrame = getElapsedFrames();
	//if (running ) {
		mCharacters.clear();
		//ON THE 26, BABY WE'LL FIND BLISS
		//currentText = secondText;
		stringIndex++;
		if (stringIndex > strings.size() - 1) stringIndex = 0;
	
	// {
}
void ReymentaTextInputTweenApp::update()
{
	int i = getElapsedFrames()-startFrame;
	if (running) {
		i = i % (currentText.size() - 1);
		if (i != currentFrame) {
			currentFrame = i;
			char c[2];
			sprintf_s(c, "%s", currentText.substr(i, 1).c_str());
			addChar(*c);
			if (!mCharacters.empty()) {
				if (mCharacters.size() > 35) {
					mCharacters.erase(mCharacters.begin(), mCharacters.begin()+10);
				}
			}
		}
	}
	else {
		if (i - startFrame < strings[stringIndex].size() - 1) {
			char c[2];
			sprintf_s(c, "%s", strings[stringIndex].substr(i - startFrame, 1).c_str());
			addChar(*c);
		}
	}


	getWindow()->setTitle("(" + toString(floor(getAverageFps())) + " fps) TextTween");

}

void ReymentaTextInputTweenApp::draw()
{
    gl::clear( Color( 0.0f, 0.0f, 0.0f ) ); 
    
	gl::setMatrices(mCam);
	gl::multModelView(mSceneMatrix().inverted());

	for (vector<Character>::iterator it = mCharacters.begin(); it != mCharacters.end(); ++it)
		it->draw();

	for (list<Character>::iterator it = mDyingCharacters.begin(); it != mDyingCharacters.end(); ++it)
		it->draw();

	if ((!mDyingCharacters.empty()) && mDyingCharacters.front().isDead())
		mDyingCharacters.pop_front();


    // -------- SPOUT -------------
    if(bInitialized) {

        // Grab the screen (current read buffer) into the local spout texture
        spoutTexture.bind();
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, g_Width, g_Height);
        spoutTexture.unbind();

        // Send the texture for all receivers to use
        // NOTE : if SendTexture is called with a framebuffer object bound, that binding will be lost
        // and has to be restored afterwards because Spout uses an fbo for intermediate rendering
        spoutsender.SendTexture(spoutTexture.getId(), spoutTexture.getTarget(), g_Width, g_Height);

    }

    // Show the user what it is sending
    //char txt[256];
    //sprintf_s(txt, "Sending as [%s]", SenderName);
    gl::setMatricesWindow( getWindowSize() );
    //gl::enableAlphaBlending();
    //gl::drawString( txt, Vec2f( toPixels( 20 ), toPixels( 20 ) ), Color( 1, 1, 1 ), Font( "Verdana", toPixels( 24 ) ) );
    //sprintf_s(txt, "fps : %2.2d", (int)getAverageFps());
    //gl::drawString( txt, Vec2f(getWindowWidth() - toPixels( 100 ), toPixels( 20 ) ), Color( 1, 1, 1 ), Font( "Verdana", toPixels( 24 ) ) );
    //gl::disableAlphaBlending();
    // ----------------------------
}
// -------- SPOUT -------------
void ReymentaTextInputTweenApp::shutdown()
{
    spoutsender.ReleaseSender();
}
Vec3f ReymentaTextInputTweenApp::getRandomVec3f(float min, float max)
{
	return Rand::randVec3f() * Rand::randFloat(min, max);
}

// This line tells Cinder to actually create the application
CINDER_APP_NATIVE( ReymentaTextInputTweenApp, RendererGl )
