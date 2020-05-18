// D:\K\Downloads\directx-sdk-samples\Direct3D11Tutorials\Tutorial06\Tutorial06.cpp
#include "window.cpp"
#include <iostream>



struct ConstantBuffer{
	Matrix wm;
	Matrix vm;
	Matrix pm;
	Vector4 l_d[2];
	Vector4 l_c[2];
	Vector4 o_c;
};



ulong vs;
ulong ps;
ulong pss;
ulong ob;
ulong cb;



void i_cb(Window* w){
	D3D11_INPUT_ELEMENT_DESC vs_inp[]={
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
		{
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			12,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};
	ulong vs=w->renderer.load_vertex_shader(L"rsrc/shader.fx","vertex_shader","vs_4_0",vs_inp,2);
	ulong ps=w->renderer.load_pixel_shader(L"rsrc/shader.fx","pixel_shader","ps_4_0");
	ulong pss=w->renderer.load_pixel_shader(L"rsrc/shader.fx","pixel_shader_solid","ps_4_0");
	cb=w->renderer.create_constant_buffer(sizeof(ConstantBuffer));
	ob=RendererHelper::create_object_buffer_box(&w->renderer,Vector3(0,0,0),2);
}



void u_cb(Window* w,double dt){
	static double t=0;
	t+=dt;
	Matrix w_m=RendererHelper::create_y_rotation_matrix(t);
	Vector4 l_d[2]={
		Vector4(-0.577f,0.577f,-0.577f,1.0f),
		Vector4(0.0f,0.0f,-1.0f,1.0f),
	};
	Vector4 l_c[2]={
		Vector4(0.5f,0.5f,0.5f,1.0f),
		Vector4(0.5f,0.0f,0.0f,1.0f)
	};
	Matrix rm=RendererHelper::create_y_rotation_matrix(-2.0f*t);
	DirectX::XMStoreFloat4(&l_d[1],DirectX::XMVector3Transform(DirectX::XMLoadFloat4(&l_d[1]),rm));
	w->renderer.clear();
	w->renderer.clear_color[2]=0.8f;
	ConstantBuffer cb1={};
	cb1.wm=RendererHelper::transpose_matrix(w_m);
	cb1.vm=RendererHelper::transpose_matrix(w->renderer.view_matrix);
	cb1.pm=RendererHelper::transpose_matrix(w->renderer.projection_matrix);
	cb1.l_d[0]=l_d[0];
	cb1.l_d[1]=l_d[1];
	cb1.l_c[0]=l_c[0];
	cb1.l_c[1]=l_c[1];
	cb1.o_c = Vector4(0, 0, 0, 0);
	w->renderer.update_constant_buffer(cb,&cb1);
	w->renderer.use_vertex_shader(vs,cb);
	w->renderer.use_pixel_shader(ps,cb);
	w->renderer.render_object_buffer(ob);
	using namespace DirectX;
	for (int i=0;i<2;i++){
		cb1.wm=RendererHelper::transpose_matrix(RendererHelper::create_scaling_matrix(0.2f,0.2f,0.2f)*RendererHelper::create_translation_matrix(5.0f*RendererHelper::create_vector(&l_d[i])));
		cb1.o_c=l_c[i];
		w->renderer.update_constant_buffer(cb,&cb1);
		w->renderer.use_pixel_shader(ps,cb);
		w->renderer.render_object_buffer(ob);
	}
	w->renderer.show();
}



int main(int argc,char** argv){
	Window w(600,600,800,600,L"Window Name",&i_cb,&u_cb);
	std::cout<<"Window Created!"<<std::endl;
	while (!w.pressed(27)){
		continue;
	}
	return 0;
}
