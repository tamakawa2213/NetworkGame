#include "Math.h"

namespace Math
{
    float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c)
    {
        return a.x * b.y * c.z +
            a.z * b.x * c.y +
            a.y * b.z * c.x -
            a.z * b.y * c.x -
            a.y * b.x * c.z -
            a.x * b.z * c.y;
    }

    bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2)
    {
        XMFLOAT3 a = XMFLOAT3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        XMFLOAT3 b = XMFLOAT3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        XMFLOAT3 d = XMFLOAT3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
        dir = XMFLOAT3(-dir.x, -dir.y, -dir.z);

        float u, v, l;
        u = Det(d, b, dir) / Det(a, b, dir);
        v = Det(a, d, dir) / Det(a, b, dir);
        l = Det(a, b, d) / Det(a, b, dir);
        if (u + v <=  1 && l >= 0 && u >= 0 && v >= 0)
        {
            return true;
        }
        return false;
    }

    bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, float* dist)
    {
        XMFLOAT3 a = XMFLOAT3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        XMFLOAT3 b = XMFLOAT3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        XMFLOAT3 d = XMFLOAT3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
        dir = XMFLOAT3(-dir.x, -dir.y, -dir.z);

        float u, v, l;
        u = Det(d, b, dir) / Det(a, b, dir);
        v = Det(a, d, dir) / Det(a, b, dir);
        l = Det(a, b, d) / Det(a, b, dir);
        if (u + v <= 1 && l >= 0 && u >= 0 && v >= 0)
        {
            if (*dist > l)
            {
                *dist = l;
                return true;
            }
            
        }
        return false;
    }

    bool SegmentToPlane(XMFLOAT3 segstart, XMFLOAT3 segend, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2)
    {
        XMFLOAT3 a = XMFLOAT3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        XMFLOAT3 b = XMFLOAT3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        XMFLOAT3 d = XMFLOAT3(segstart.x - v0.x, segstart.y - v0.y, segstart.z - v0.z);

        XMFLOAT3 dir = XMFLOAT3(segend.x - segstart.x, segend.y - segstart.y, segend.z - segstart.z);
        float dist;
        dist = (float)sqrt(pow(dir.x, 2.0) + pow(dir.y, 2.0) + pow(dir.z, 2.0));

        XMVECTOR vDir = XMLoadFloat3(&dir);
        vDir = XMVector3Normalize(vDir);
        XMStoreFloat3(&dir, vDir);

        dir = XMFLOAT3(-dir.x, -dir.y, -dir.z);

        float u, v, l;
        u = Det(d, b, dir) / Det(a, b, dir);
        v = Det(a, d, dir) / Det(a, b, dir);
        l = Det(a, b, d) / Det(a, b, dir);
        if (u + v <= 1 && l >= 0 && u >= 0 && v >= 0 && l < dist)
        {
            return true;
        }
        return false;
    }

    int GetDigits(int value, int m, int n)
    {
        int val = n + 1;
        int result;

        //n桁目以下の桁を取得
        val = value % (int)pow(10, val);

        //m桁目以上の桁を取得
        result = val / pow(10, m);

        return result;
    }

    std::vector<int> MakePrime(int article)
    {
        std::vector<int> ans;   //最終的に答えとなる配列
        ans.clear();            //初期化
        ans.reserve(article);   //配列の要素数を確保

        std::vector<int> sieve; //エラトステネスの篩
        sieve.clear();          //初期化
        sieve.push_back(2);     //2から検索開始する

        ans.push_back(2);       //呼び出しは1以上の値を前提とするので予め2を入れる
        
        int PrimeNumber = 3;    //検索対象の数値 : 初期値は3にする

        bool IsPrime = true;   //検索対象が素数かどうかを判定させる

        //配列の要素数が指定した数を超えるまで挿入
        while ((int)ans.size() < article)
        {
            //ansがsieveより要素数が多い場合に呼び出す
            if (ans.size() > sieve.size())
            {
                //sieveの最後尾の値 = ansのsieve.size()に格納されている値なので
                //その次の値の二乗が検索対象より小さければ
                if (PrimeNumber >= pow((double)ans.at(sieve.size()), 2))
                {
                    sieve.push_back(ans.at(sieve.size()));  //sieveにその値を格納
                }
            }

            //sieveの要素毎に判定させる
            for (int i = 0; i < sieve.size(); i++)
            {
                //sieveの要素の値で割り切れたら
                if (PrimeNumber % sieve.at(i) == 0)
                {
                    IsPrime = false;    //IsPrimeを倒して
                    break;              //強制終了
                }
            }

            if (IsPrime)   //検索対象が素数ならば
            {
                ans.push_back(PrimeNumber); //要素に追加
            }

            IsPrime = true; //IsPrimeを強制的に立たせる

            //偶数の素数が2以外に存在しないので
            PrimeNumber += 2;   //PrimeNumberに2を加算させる
        }
        return ans;
    }
}