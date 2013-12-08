#ifndef FORM_H_
#define FORM_H_

#include "Ref.h"
#include "Container.h"
#include "Mesh.h"
#include "Node.h"
#include "Touch.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"
#include "FrameBuffer.h"

namespace gameplay
{

class Theme;

/**
 * Top-level container of UI controls.
 
   Child controls and containers can be created and added to a form using the
   Container::addControl and Container::insertControl methods.

   The following properties are available for forms:

 @verbatim
    form <formID>
    {
        // Form properties.
        theme       = <Path to .theme File> // See Theme.h.
        layout      = <Layout::Type>        // A value from the Layout::Type enum.  e.g.: LAYOUT_VERTICAL
        style       = <styleID>             // A style from the referenced theme.
        position   = <x, y>                // Position of the form on-screen, measured in pixels.
        alignment  = <Control::Alignment>  // Note: 'position' will be ignored.
        autoWidth  = <bool>                // Will result in a form the width of the display.
        autoHeight = <bool>                // Will result in a form the height of the display.
        size       = <width, height>       // Size of the form, measured in pixels.
        width      = <width>               // Can be used in place of 'size', e.g. with 'autoHeight = true'
        height     = <height>              // Can be used in place of 'size', e.g. with 'autoWidth = true'
        consumeEvents = <bool>             // Whether the form propagates input events to the Game's input event handler. Default is false
      
        // All the nested controls within this form.
        container { }
        label { }
        textBox { }
        button { }
        checkBox { }
        radioButton { }
        slider { }
    }
 @endverbatim
 */
class Form : public Container
{
    friend class Platform;
    friend class Game;
    friend class Gamepad;
    friend class Control;
    friend class Container;

public:

    /**
     * Creates a form using the data from the Properties object defined at the specified URL, 
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional). 
     * 
     * @param url The URL pointing to the Properties object defining the form. 
     * @script{create}
     */
    static Form* create(const char* url);

    /**
     * Create a new Form.
	 *
	 * The specified style defines the visual style for the form. If NULL is passed
	 * for the style, the default UI theme is used. All controls attached to this
	 * form will inherit the theme that contains the form's style.
     *
     * @param id The Form's ID.
     * @param style The Form's custom style (optional - may be NULL).
	 * @param layoutType The form's layout type (optional).
     *
     * @return The new Form.
     * @script{create}
     */
    static Form* create(const char* id, Theme::Style* style, Layout::Type layoutType = Layout::LAYOUT_ABSOLUTE);

    /**
     * Get a form from its ID.
     *
     * @param id The ID of the form to search for.
     *
     * @return A form with the given ID, or null if one was not found.
     */
    static Form* getForm(const char* id);
    
    /**
     * Returns the current UI control that is in focus.
     *
     * @return The current control in focus, or NULL if no controls are in focus.
     */
    static Control* getFocusControl();

    /**
     * Removes focus from any currently focused UI control.
     */
    static void clearFocus();

    /**
     * @see Container#isForm()
     */
    bool isForm() const;

    /**
     * Attach this form to a node.
     *
     * A form can be drawn as part of the 3-dimensional world if it is attached to a node.
     *
     * @param node The node to attach this form to.
     */
    void setNode(Node* node);

    /**
     * Updates each control within this form, and positions them according to its layout.
     */
    void update(float elapsedTime);

    /**
     * Draws this form.
     *
     * @return The nubmer of draw calls issued to draw the form.
     */
    unsigned int draw();

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Determines whether batching is enabled for this form.
     *
     * @return True if batching is enabled for this form, false otherwise.
     */
    bool isBatchingEnabled() const;

    /**
     * Turns batching on or off for this form.
     *
     * By default, forms enable batching as a way to optimize performance. However, on certain
     * complex forms that contain multiple layers of overlapping text and transparent controls,
     * batching may cause some visual artifacts due alpha blending issues. In these cases,
     * turning batching off usually fixes the issue at a slight performance cost.
     *
     * @param enabled True to enable batching (default), false otherwise.
     */
    void setBatchingEnabled(bool enabled);

    /**
     * Returns the single currently active control within the UI system.
     *
     * @return The currently active control, or NULL if no controls are currently active.
     */
    static Control* getActiveControl();

protected:

    /**
     * @see Control::update
     */
    void update(const Control* container, const Vector2& offset);

private:
    
    /**
     * Constructor.
     */
    Form();

    /**
     * Constructor.
     */
    Form(const Form& copy);

    /**
     * Destructor.
     */
    virtual ~Form();

    /**
     * @see Control::initialize
     */
    void initialize(const char* typeName, Theme::Style* style, Properties* properties);

    /**
     * Initialize a quad for this form in order to draw it in 3D.
     *
     * @param mesh The mesh to create a model from.
     */
    void initializeQuad(Mesh* mesh);

    /**
     * Updates all visible, enabled forms.
     */
    static void updateInternal(float elapsedTime);

    /**
     * Propagate touch events to enabled forms.
     *
     * @return Whether the touch event was consumed by a form.
     */
    static bool touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Propagate key events to enabled forms.
     *
     * @return Whether the key event was consumed by a form.
     */
    static bool keyEventInternal(Keyboard::KeyEvent evt, int key);

    /**
     * Propagate mouse events to enabled forms.
     *
     * @return True if the mouse event is consumed or false if it is not consumed.
     *
     * @see Mouse::MouseEvent
     */
    static bool mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * Propagate gamepad events to enabled forms.
     *
     * @see Control::gamepadEvent
     */
    static bool gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex);

    /**
     * Fired by the platform when the game window resizes.
     *
     * @param width The new window width.
     * @param height The new window height.
     */
    static void resizeEventInternal(unsigned int width, unsigned int height);

    /**
     * Called to update internal framebuffer when forms are attached to a node.
     */
    void updateFrameBuffer();

    /**
     * Called during drawing to prepare a sprite batch for being drawn into for this form.
     */
    void startBatch(SpriteBatch* batch);

    /**
     * Called during drawing to signal completion of drawing into a batch.
     */
    void finishBatch(SpriteBatch* batch);

    /**
     * Unproject a point (from a mouse or touch event) into the scene and then project it onto the form.
     *
     * @param x The x coordinate of the mouse/touch point.
     * @param y The y coordinate of the mouse/touch point.
     * @param point A destination vector to populate with the projected point, in the form's plane.
     *
     * @return True if the projected point lies within the form's plane, false otherwise.
     */
    bool projectPoint(int x, int y, Vector3* point);

    const Matrix& getProjectionMatrix() const;

    static bool pointerEventInternal(bool mouse, int evt, int x, int y, int param);

    static Control* findInputControl(int* x, int* y, bool focus);

    static Control* findInputControl(Control* control, int x, int y, bool focus);

    static Control* handlePointerPressRelease(int* x, int* y, bool pressed);

    static Control* handlePointerMove(int* x, int* y);

    static bool screenToForm(Control* ctrl, int* x, int* y);

    static void verifyRemovedControlState(Control* control);

    static void controlDisabled(Control* control);

    static void setFocusControl(Control* control);

    static void pollGamepads();

    static bool pollGamepad(Gamepad* gamepad);

    Node* _node;                        // Node for transforming this Form in world-space.
    FrameBuffer* _frameBuffer;          // FrameBuffer for offscreen drawing of forms that are attached to a Node
    Model* _model;                      // Model used to render form in 3D when attached to a Node
    Matrix _projectionMatrix;           // Projection matrix to be set on SpriteBatch objects when rendering the form
    std::vector<SpriteBatch*> _batches;
    bool _batched;
    static Control* _focusControl;
    static Control* _activeControl;
    static Control::State _activeControlState;
};

}

#endif
