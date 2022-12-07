#include "Picture.h"
#include "CallDef.h"

namespace Picture
{
    std::vector<PictureSet*> FileSet;      //Fbxの構造体の動的配列

    int Picture::Load(LPCWSTR filename)
    {
        HRESULT hr;
        PictureSet* File = new PictureSet;
        File->FileName = filename;
        for (auto itr = FileSet.begin(); itr != FileSet.end(); itr++)
        {
            //同じ名前のファイルをすでにロードしていた場合
            if (File->FileName == (*itr)->FileName)
            {
                File->pSprite = (*itr)->pSprite;
                File->FindFbx = true;
                break;
            }
        }
        //見つからなかった場合、新しくロードする
        if (!File->FindFbx)
        {
            File->pSprite = new Sprite;
            hr = File->pSprite->Initialize(filename);
            if (FAILED(hr)) //ロードに失敗した場合
            {
                SAFE_DELETE(File->pSprite);
                SAFE_DELETE(File);
            }
        }

        FileSet.push_back(File);
        return (int)FileSet.size() - 1;
    }

    void Picture::SetTransform(int hPicture, Transform transform)
    {
        FileSet[hPicture]->transform = transform;
    }

    void Picture::Draw(int hModel)
    {
        FileSet[hModel]->pSprite->Draw(FileSet[hModel]->transform);
    }

    void Release()
    {
        for (int i = 0; i < FileSet.size(); i++)
        {
            if (!FileSet[i]->FindFbx)
            {
                SAFE_DELETE(FileSet[i]->pSprite);
            }
            SAFE_DELETE(FileSet[i]);
        }
        FileSet.clear();
    }
}