const App = {};

async function initModule() {
    if (SdglAppModule === undefined)
    {
        throw Error("Cannot start app because the module it not loaded");
    }

    return new Promise((resolve, reject) => {
        const timeout = setTimeout(reject, 15000);

        App.onRuntimeInitialized = () => {
            clearTimeout(timeout);
            resolve(App);
        };

        App.canvas = (() => {
            const canvas = document.getElementById("canvas");

            // Default behavior pops up alert when webgl context is lost.
            // TODO: To make app more robust, you may want to override this behavior before shipping!
            // See http://www.khronos.org/registry/webgl/specs/latest/1.0/#5.15.2
            canvas.addEventListener("webglcontextlost", e => {
                alert("WebGL context lost. Please reload the page");

                e.preventDefault();
            }, false);

            canvas.addEventListener("webglcontextrestored", e => {
                // TODO: reinitialize graphics resources while maintaining game state here
                // expose some c++ function via bindings to handle this
            }, false);

            return canvas;
        })();

        SdglAppModule(App);
    });
}

initModule()
    .catch(e => {
        console.error("Failed to initialize app");
        if (e)
            console.error(e);
    });
