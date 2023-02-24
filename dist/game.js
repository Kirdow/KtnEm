Module.onRuntimeInitialized = function() {
    Module.Start(window.innerHeight * 0.7);

    (window || globalThis).GetScale = () => Module.GetScale();
    (window || globalThis).SetScale = (scale) => Module.SetScale(scale);
    (window || globalThis).SetSize = (height) => {
        Module.SetSize(0, height);
    };
    (window || globalThis).GetSize = () => Module.GetSize();
}