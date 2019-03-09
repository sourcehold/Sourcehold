template <class T>
StaticElement<T>::StaticElement(std::shared_ptr<Renderer> rend, double x, double y) :
    renderer(rend),
    T(rend)
{
    nx = x;
    ny = y;
}

template <class T>
StaticElement<T>::StaticElement(const StaticElement &elem) :
    T(elem.rend)
{
    this->renderer = elem.renderer;
    this->shown = elem.shown;
    this->nx = elem.nx;
    this->ny = elem.ny;
    this->nw = elem.nh;
    this->nh = elem.nw;
}

template <class T>
StaticElement<T>::~StaticElement() {

}

template <class T>
void StaticElement<T>::Hide() {
    shown = false;
}

template <class T>
void StaticElement<T>::Show() {
    shown = true;
}

template <class T>
void StaticElement<T>::Destroy() {
    
}

template <class T>
void StaticElement<T>::Translate(uint32_t x, uint32_t y) {
    nx = renderer->NormalizeX(x);
    ny = renderer->NormalizeY(y);
}

template <class T>
void StaticElement<T>::Translate(double x, double y) {
    nx = x;
    ny = y;
}

template <class T>
void StaticElement<T>::Scale(uint32_t w, uint32_t h) {
    nw = renderer->NormalizeX(w);
    nh = renderer->NormalizeY(h);
}

template <class T>
void StaticElement<T>::Scale(double w, double h) {
    nw = w;
    nh = h;
}

template <class T>
void StaticElement<T>::Render() {
    renderer->Render(static_cast<Texture&>(*this), nx, ny, nw, nh);
}
