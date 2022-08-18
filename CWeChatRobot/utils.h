#pragma once
#include <set>
#include <utility>
#include <chrono>

#define CHRONO std::chrono
typedef unsigned long long ull;

/*
 * һ���򵥵Ĵ�����ʱ���������ļ��ϣ�intervalΪ����
 * ExpireSet es(1000); // ����ʱ��1��
 * es.CheckIfDuplicatedAndAdd(1); true, δ�ظ�
 * es.CheckIfDuplicatedAndAdd(1); false, �ظ�
 * After 1s;
 * es.CheckIfDuplicatedAndAdd(2); ���<2>��������ڵ�Ԫ��<1>
 */
class ExpireSet
{
public:
    ull interval; // ����
    ExpireSet(ull interval)
    {
        this->interval = interval;
    }
    bool CheckIfDuplicatedAndAdd(ull id)
    {
        bool ok = true;
        if (ids.count(id) != 0)
        {
            ok = false;
        }
        Add(id);
        return ok;
    }

private:
    std::set<ull> ids;
    std::pair<ull, ull> p;

    void Add(ull id)
    {
        // ����
        auto now = CHRONO::time_point_cast<CHRONO::milliseconds>(CHRONO::system_clock::now());
        if (p.second < now.time_since_epoch().count())
        {
            ids.clear();
            p.first = now.time_since_epoch().count();
            p.second = p.first + interval;
        }
        ids.insert(id);
    }
};
