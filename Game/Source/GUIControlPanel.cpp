#include "GUIControlPanel.h"
#include "GUIVertex.h"

namespace T3{
	namespace GUI{
		namespace Control{
			void	AddVertexs(const CB::Math::CRectangleF32& Rect, CB::Collection::CList<CVertex>& verts){
				verts.Add(CVertex(Rect.GetXLower(), Rect.GetYLower(), 0.0f, 0.0f, 0.0f));
				verts.Add(CVertex(Rect.GetXHigher(), Rect.GetYLower(), 0.0f, 1.0f, 0.0f));
				verts.Add(CVertex(Rect.GetXHigher(), Rect.GetYHigher(), 0.0f, 1.0f, 1.0f));
				verts.Add(CVertex(Rect.GetXLower(), Rect.GetYHigher(), 0.0f, 0.0f, 1.0f));				
			}

			CPanel::CPanel(IScreen& Screen, const CB::CString& strID) :
				CControlBase(Screen, strID)
			{
				CB::Collection::CList<CVertex> verts;
				CB::Collection::CList<uint16> idxs;

				AddVertexs(this->m_Rect, verts);
				idxs.Add(0); idxs.Add(2); idxs.Add(1);
				idxs.Add(0); idxs.Add(3); idxs.Add(2);

				{
					using namespace CB::Graphic;
					this->m_pVertexBuffer = Screen.GetDevice()->CreateBuffer(BufferType::Vertex, BufferUsage::Static, BufferAccess::Write, verts);
					this->m_pIndexBuffer = Screen.GetDevice()->CreateBuffer(BufferType::Index, BufferUsage::Static, BufferAccess::Write, idxs);
				}
			}

			void	CPanel::SetRect(const CB::Math::CRectangleF32& Rect){
				this->m_Rect;
				CB::Collection::CList<CVertex> verts;
				AddVertexs(this->m_Rect, verts);
				this->m_pVertexBuffer->LoadSubData(verts, 0);
			}

			const CB::Math::CRectangleF32& CPanel::GetRect() const{
				return this->m_Rect;
			}

			void	CPanel::Render(CB::CRefPtr<CB::Graphic::IDevice> pDevice){
				pDevice->SetVertexBuffer(0, this->m_pVertexBuffer);
				pDevice->SetIndexBuffer(this->m_pIndexBuffer);

				pDevice->RenderIndexed(2);

				CControlBase::Render(pDevice);
			}
		}
	}
}