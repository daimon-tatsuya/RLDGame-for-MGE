#pragma once
#include <windows.h>
#include <wrl.h>
#include <directXMath.h>
#include <d3d11.h>
#include <memory>
class Scene
{
public:

	Scene() {}
	virtual ~Scene() {}
	// ������
	virtual void Initialize() = 0;
	// �I����
	virtual void Finalize() = 0;
	// �X�V����
	virtual void Update(float elapsedTime) = 0;
	// �`�揈��
	virtual void Render() = 0;
	// �����������Ă��邩
	bool IsReady() const { return ready; }
	// ���������ݒ�
	void SetReady() { ready = true; }
private:

public:

private:
	bool	ready = false;
};
