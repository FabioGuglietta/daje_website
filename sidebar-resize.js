(() => {
  const STORAGE_KEY = "daje.sidebar.width";
  const MIN_WIDTH = 220;
  const MAX_WIDTH = 420;
  const SIDEBAR_SELECTOR = ".myst-primary-sidebar-pointer";

  const clamp = (value) => Math.min(MAX_WIDTH, Math.max(MIN_WIDTH, value));

  function initSidebar(sidebar) {
    if (!sidebar || sidebar.dataset.resizeInit === "1") return false;
    sidebar.dataset.resizeInit = "1";

    let handle = sidebar.querySelector(".custom-sidebar-handle");
    if (!handle) {
      handle = document.createElement("div");
      handle.className = "custom-sidebar-handle";
      handle.setAttribute("role", "separator");
      handle.setAttribute("aria-label", "Resize sidebar");
      handle.setAttribute("aria-orientation", "vertical");
      sidebar.appendChild(handle);
    }

    const savedWidth = Number.parseFloat(localStorage.getItem(STORAGE_KEY) || "");
    if (Number.isFinite(savedWidth)) {
      sidebar.style.width = `${clamp(savedWidth)}px`;
    }

    let dragging = false;

    const onPointerMove = (event) => {
      if (!dragging) return;
      const left = sidebar.getBoundingClientRect().left;
      const width = clamp(event.clientX - left);
      sidebar.style.width = `${width}px`;
      localStorage.setItem(STORAGE_KEY, String(width));
    };

    const onPointerUp = () => {
      dragging = false;
      document.body.classList.remove("sidebar-dragging");
      window.removeEventListener("pointermove", onPointerMove);
      window.removeEventListener("pointerup", onPointerUp);
    };

    handle.addEventListener("pointerdown", (event) => {
      if (event.button !== 0) return;
      event.preventDefault();
      dragging = true;
      document.body.classList.add("sidebar-dragging");
      window.addEventListener("pointermove", onPointerMove);
      window.addEventListener("pointerup", onPointerUp);
    });

    return true;
  }

  function init() {
    const tryInit = () => initSidebar(document.querySelector(SIDEBAR_SELECTOR));
    if (tryInit()) return;

    const observer = new MutationObserver(() => {
      if (tryInit()) observer.disconnect();
    });

    observer.observe(document.documentElement, {
      childList: true,
      subtree: true,
    });

    // Safety timeout to avoid keeping the observer forever.
    window.setTimeout(() => observer.disconnect(), 10000);
  }

  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", init);
  } else {
    init();
  }
})();
