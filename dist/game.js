Module.onRuntimeInitialized = function() {
    Module.Start(window.innerHeight * 0.7);

    (window || globalThis).GetScale = () => Module.GetScale();
    (window || globalThis).SetScale = (scale) => Module.SetScale(scale);
    (window || globalThis).SetHeight = (height) => {
        Module.SetHeight(height);
    };
    (window || globalThis).GetSize = () => Module.GetSize();
}