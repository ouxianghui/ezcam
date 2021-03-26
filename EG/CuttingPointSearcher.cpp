#include "CuttingPointSearcher.h"


CuttingPointSearcher::CuttingPointSearcher(double dOffset)
{
    this->m_dOffset = dOffset;
    this->m_dDistance = 500.0;
    this->m_dTolerance = 0.01;
}

CuttingPointSearcher::~CuttingPointSearcher()
{

}

void CuttingPointSearcher::search(std::vector<RectF> &vecRect)
{
    sortByArea(vecRect);
    sortByXY(vecRect);
    sortByPosition(vecRect);
}

RVector CuttingPointSearcher::search(const RectF &s, const std::vector<RectF> &vec, std::vector<RectF> &vecDeletedRect)
{
    std::vector<RectF> vecAboveRect;
    std::vector<RectF> vecBelowRect;
    std::vector<RectF> vecRightRect;
    std::vector<RectF> vecLeftRect;

    for (size_t j = 0; j < vec.size(); j++)
    {//上下左右有效矩形
        RectF rcJ = vec.at(j);
        if(s != rcJ)
        {
            if( isAbove(s, rcJ) )
            {
                vecAboveRect.push_back(rcJ);
            }
            else if(isBelow(s, rcJ))
            {
                vecBelowRect.push_back(rcJ);
            }
            else if(isLeft(s, rcJ))
            {
                vecLeftRect.push_back(rcJ);
            }
            else if(isRight(s, rcJ))
            {
                vecRightRect.push_back(rcJ);
            }
        }
    }

    for(size_t i = 0; i <vecAboveRect.size(); i++)
    {//根据离矩形的距离，对矩形进行排序
        RectF &rcI = vecAboveRect.at(i);
        for(size_t j = i+1; j < vecAboveRect.size(); j++)
        {
           RectF &rcJ = vecAboveRect.at(j);
           if((rcI.y + rcI.height) < (rcJ.y + rcJ.height))
           {
               RectF rect = rcI;
               rcI = rcJ;
               rcJ = rect;
           }
        }
    }

    for(size_t i = 0; i <vecBelowRect.size(); i++)
    {//根据离矩形的距离，对矩形进行排序
        RectF &rcI = vecBelowRect.at(i);
        for(size_t j = i+1; j < vecBelowRect.size(); j++)
        {
           RectF &rcJ = vecBelowRect.at(j);
           if(rcI.y  > rcJ.y )
           {
               RectF rect = rcI;
               rcI = rcJ;
               rcJ = rect;
           }
        }
    }

    for(size_t i = 0; i <vecLeftRect.size(); i++)
    {//根据离矩形的距离，对矩形进行排序
        RectF &rcI = vecLeftRect.at(i);
        for(size_t j = i+1; j < vecLeftRect.size(); j++)
        {
           RectF &rcJ = vecLeftRect.at(j);
           if((rcI.x + rcI.width) < (rcJ.x + rcJ.width))
           {
               RectF rect = rcI;
               rcI = rcJ;
               rcJ = rect;
           }
        }
    }

    for(size_t i = 0; i <vecRightRect.size(); i++)
    {//根据离矩形的距离，对矩形进行排序
        RectF &rcI = vecRightRect.at(i);
        for(size_t j = i+1; j < vecRightRect.size(); j++)
        {
           RectF &rcJ = vecRightRect.at(j);
           if( rcI.x  > rcJ.x )
           {
               RectF rect = rcI;
               rcI = rcJ;
               rcJ = rect;
           }
        }
    }

    std::vector<RectF> vecEffectiveAboveRect;
    std::vector<XSegment> vecXsegment;
    std::vector<std::vector<XSegment>> vecEffectiveAboveXsegment;

    for(size_t i = 0; i < vecAboveRect.size(); i++)
    {//计算出上方各个有效矩形的线段
        RectF r = vecAboveRect.at(i);

        //先计算r与s之间有效的线段
        XSegment xs;
        if(r.x <= s.x && (r.x + r.width) >= (s.x + s.width))
        {
            xs.startX = s.x;
            xs.endX = s.x + s.width;
        }
        else if(r.x <= s.x && (r.x + r.width) >= s.x && (r.x + r.width) <= (s.x + s.width))
        {
            xs.startX = s.x;
            xs.endX = r.x + r.width;
        }
        else if(r.x >= s.x && (r.x + r.width) <= (s.x + s.width))
        {
            xs.startX = r.x;
            xs.endX = r.x + r.width;
        }
        else if(r.x >= s.x && r.x <= (s.x + s.width) && (r.x + r.width) >= (s.x + s.width))
        {
            xs.startX = r.x;
            xs.endX = s.x + s.width;
        }

        //去掉重合的部分
        if(vecEffectiveAboveRect.empty())
        {//第一个矩形，该边全部有效
            vecEffectiveAboveRect.push_back(r);

            vecXsegment.push_back(xs);
            vecEffectiveAboveXsegment.push_back(vecXsegment);
        }
        else
        {
            std::vector<XSegment> vecXSAdd;
            vecXSAdd.push_back(xs);

            std::vector<std::vector<XSegment>>::iterator iterAboveXsegment = vecEffectiveAboveXsegment.begin();
            std::vector<RectF>::iterator iterAboveRect = vecEffectiveAboveRect.begin();
            while(iterAboveRect != vecEffectiveAboveRect.end())
            {//与有效的矩形比较，去掉重合的部分

                std::vector<XSegment> vecXS = *iterAboveXsegment;

                for(size_t k = 0; k < vecXS.size(); k++)
                {//已存在的矩形对应的有效线段
                    XSegment xsk = vecXS.at(k);

                    std::vector<XSegment>::iterator iterXSAdd = vecXSAdd.begin();
                    while(iterXSAdd != vecXSAdd.end())
                    {//新增加的相邻矩形的有效线段
                        XSegment &xsl = *iterXSAdd;

                        if(xsl.startX < xsk.startX && xsl.endX >= xsk.startX && xsl.endX < xsk.endX)
                        {
                            xsl.endX = xsk.startX;
                            iterXSAdd++;
                        }
                        else if (xsl.startX >= xsk.startX && xsl.endX <= xsk.endX)
                        {//被已有的边包含，不计算
                            iterXSAdd = vecXSAdd.erase(iterXSAdd);
                        }
                        else if (xsl.startX >= xsk.startX && xsl.startX <= xsk.endX && xsl.endX > xsk.endX)
                        {
                            xsl.startX = xsk.endX;
                            iterXSAdd++;
                        }
                        else if (xsl.startX < xsk.startX && xsl.endX > xsk.endX)
                        {
                            XSegment xsAdd;
                            xsAdd.startX = xsk.endX;
                            xsAdd.endX = xsl .endX;
                            xsl.endX = xsk.startX;

                            vecXSAdd.push_back(xsAdd);
                            iterXSAdd = vecXSAdd.begin();
                        }
                        else
                        {
                            iterXSAdd++;
                        }
                    }
                }

                iterAboveRect++;
                iterAboveXsegment++;
            }

            if(!vecXSAdd.empty())
            {
                vecEffectiveAboveRect.push_back(r);
                vecEffectiveAboveXsegment.push_back(vecXSAdd);
            }

        }
    }

    //below========
    std::vector<RectF> vecEffectiveBelowRect;
    std::vector<XSegment>().swap(vecXsegment);
    std::vector<std::vector<XSegment>> vecEffectiveBelowXsegment;

    for(size_t i = 0; i < vecBelowRect.size(); i++)
    {//计算出下方各个有效矩形的线段
        RectF r = vecBelowRect.at(i);

        //先计算r与s之间有效的线段
        XSegment xs;
        if(r.x <= s.x && (r.x + r.width) >= (s.x + s.width))
        {
            xs.startX = s.x;
            xs.endX = s.x + s.width;
        }
        else if(r.x <= s.x && (r.x + r.width) >= s.x && (r.x + r.width) <= (s.x + s.width))
        {
            xs.startX = s.x;
            xs.endX = r.x + r.width;
        }
        else if(r.x >= s.x && (r.x + r.width) <= (s.x + s.width))
        {
            xs.startX = r.x;
            xs.endX = r.x + r.width;
        }
        else if(r.x >= s.x && r.x <= (s.x + s.width) && (r.x + r.width) >= (s.x + s.width))
        {
            xs.startX = r.x;
            xs.endX = s.x + s.width;
        }

        //去掉重合的部分
        if(vecEffectiveBelowRect.empty())
        {//第一个矩形，该边全部有效
            vecEffectiveBelowRect.push_back(r);

            vecXsegment.push_back(xs);
            vecEffectiveBelowXsegment.push_back(vecXsegment);
        }
        else
        {
            std::vector<XSegment> vecXSAdd;
            vecXSAdd.push_back(xs);

            std::vector<std::vector<XSegment>>::iterator iterBelowXsegment = vecEffectiveBelowXsegment.begin();
            std::vector<RectF>::iterator iterBelowRect = vecEffectiveBelowRect.begin();

            while(iterBelowRect != vecEffectiveBelowRect.end())
            {//与有效的矩形比较，去掉重合的部分


                std::vector<XSegment> vecXS = *iterBelowXsegment;

                for(size_t k = 0; k < vecXS.size(); k++)
                {//已存在的矩形对应的有效线段
                    XSegment xsk = vecXS.at(k);

                    std::vector<XSegment>::iterator iterXSAdd = vecXSAdd.begin();
                    while(iterXSAdd != vecXSAdd.end())
                    {//新增加的相邻矩形的有效线段
                        //XSegment &xsl = vecXSAdd.at(l);
                        XSegment &xsl = *iterXSAdd;
                        if(xsl.startX < xsk.startX && xsl.endX >= xsk.startX && xsl.endX < xsk.endX)
                        {
                            xsl.endX = xsk.startX;
                            iterXSAdd++;
                        }
                        else if (xsl.startX >= xsk.startX && xsl.endX <= xsk.endX)
                        {//被已有的边包含，不计算
                            iterXSAdd = vecXSAdd.erase(iterXSAdd);
                        }
                        else if (xsl.startX >= xsk.startX && xsl.startX <= xsk.endX && xsl.endX > xsk.endX)
                        {
                            xsl.startX = xsk.endX;
                            iterXSAdd++;
                        }
                        else if (xsl.startX < xsk.startX && xsl.endX > xsk.endX)
                        {
                            XSegment xsAdd;
                            xsAdd.startX = xsk.endX;
                            xsAdd.endX = xsl .endX;
                            xsl.endX = xsk.startX;

                            vecXSAdd.push_back(xsAdd);
                            iterXSAdd = vecXSAdd.begin();

                        }
                        else
                        {
                            iterXSAdd++;
                        }
                    }
                }

                iterBelowRect++;
                iterBelowXsegment++;
            }

            if(!vecXSAdd.empty())
            {
                vecEffectiveBelowRect.push_back(r);
                vecEffectiveBelowXsegment.push_back(vecXSAdd);
            }
        }
    }

    //below-end========

    //left==============

    std::vector<RectF> vecEffectiveLeftRect;
    std::vector<YSegment> vecYsegment;
    std::vector<std::vector<YSegment>> vecEffectiveLeftYsegment;

    for(size_t i = 0; i < vecLeftRect.size(); i++)
    {//计算出下方各个有效矩形的线段
        RectF r = vecLeftRect.at(i);

        //先计算r与s之间有效的线段
        YSegment ys;
        if(r.y <= s.y && (r.y + r.height) >= (s.y + s.height))
        {
            ys.startY = s.y;
            ys.endY = s.y + s.height;
        }
        else if(r.y <= s.y && (r.y + r.height) >= s.y && (r.y + r.height) <= (s.y + s.height))
        {
            ys.startY = s.y;
            ys.endY = r.y + r.height;
        }
        else if(r.y >= s.y && (r.y + r.height) <= (s.y + s.height))
        {
            ys.startY = r.y;
            ys.endY = r.y + r.height;
        }
        else if(r.y >= s.y && r.y <= (s.y + s.height) && (r.y + r.height) >= (s.y + s.height))
        {
            ys.startY = r.y;
            ys.endY = s.y + s.height;
        }

        //去掉重合的部分
        if(vecEffectiveLeftRect.empty())
        {//第一个矩形，该边全部有效
            vecEffectiveLeftRect.push_back(r);

            vecYsegment.push_back(ys);
            vecEffectiveLeftYsegment.push_back(vecYsegment);
        }
        else
        {
            std::vector<YSegment> vecYSAdd;
            vecYSAdd.push_back(ys);

            std::vector<std::vector<YSegment>>::iterator iterLeftYsegment = vecEffectiveLeftYsegment.begin();
            std::vector<RectF>::iterator iterLeftRect = vecEffectiveLeftRect.begin();

            while(iterLeftRect != vecEffectiveLeftRect.end())
            {//与有效的矩形比较，去掉重合的部分

                std::vector<YSegment> vecYS = *iterLeftYsegment;

                for(size_t k = 0; k < vecYS.size(); k++)
                {//已存在的矩形对应的有效线段
                    YSegment ysk = vecYS.at(k);

                    std::vector<YSegment>::iterator iterYSAdd = vecYSAdd.begin();

                    while(iterYSAdd != vecYSAdd.end())
                    {//新增加的相邻矩形的有效线段
                        YSegment &ysl = *iterYSAdd;

                        if(ysl.startY < ysk.startY && ysl.endY >= ysk.startY && ysl.endY < ysk.endY)
                        {
                            ysl.endY = ysk.startY;
                            iterYSAdd++;
                        }
                        else if (ysl.startY >= ysk.startY && ysl.endY <= ysk.endY)
                        {
                            //被已有的边包含，不计算
                            iterYSAdd = vecYSAdd.erase(iterYSAdd);
                        }
                        else if (ysl.startY >= ysk.startY && ysl.startY <= ysk.endY && ysl.endY > ysk.endY)
                        {
                            ysl.startY = ysk.endY;
                            iterYSAdd++;
                        }
                        else if (ysl.startY < ysk.startY && ysl.endY > ysk.endY)
                        {
                            YSegment ysAdd;
                            ysAdd.startY = ysk.endY;
                            ysAdd.endY = ysl.endY;
                            ysl.endY = ysk.startY;

                            vecYSAdd.push_back(ysAdd);
                            iterYSAdd = vecYSAdd.begin();
                        }
                        else
                        {
                            iterYSAdd++;
                        }
                    }
                }
                iterLeftRect++;
                iterLeftYsegment++;
            }

            if(!vecYSAdd.empty())
            {
                vecEffectiveLeftRect.push_back(r);
                vecEffectiveLeftYsegment.push_back(vecYSAdd);
            }

        }
    }

    //==============

    //right==============

    std::vector<RectF> vecEffectiveRightRect;
    std::vector<YSegment>().swap(vecYsegment);
    std::vector<std::vector<YSegment>> vecEffectiveRightYsegment;

    for(size_t i = 0; i < vecRightRect.size(); i++)
    {//计算出下方各个有效矩形的线段
        RectF r = vecRightRect.at(i);

        //先计算r与s之间有效的线段
        YSegment ys;
        if(r.y <= s.y && (r.y + r.height) >= (s.y + s.height))
        {
            ys.startY = s.y;
            ys.endY = s.y + s.height;
        }
        else if(r.y <= s.y && (r.y + r.height) >= s.y && (r.y + r.height) <= (s.y + s.height))
        {
            ys.startY = s.y;
            ys.endY = r.y + r.height;
        }
        else if(r.y >= s.y && (r.y + r.height) <= (s.y + s.height))
        {
            ys.startY = r.y;
            ys.endY = r.y + r.height;
        }
        else if(r.y >= s.y && r.y <= (s.y + s.height) && (r.y + r.height) >= (s.y + s.height))
        {
            ys.startY = r.y;
            ys.endY = s.y + s.height;
        }

        //去掉重合的部分
        if(vecEffectiveRightRect.empty())
        {//第一个矩形，该边全部有效
            vecEffectiveRightRect.push_back(r);

            vecYsegment.push_back(ys);
            vecEffectiveRightYsegment.push_back(vecYsegment);
        }
        else
        {
            std::vector<YSegment> vecYSAdd;
            vecYSAdd.push_back(ys);

            std::vector<std::vector<YSegment>>::iterator iterRightYsegment = vecEffectiveRightYsegment.begin();
            std::vector<RectF>::iterator iterRightRect = vecEffectiveRightRect.begin();

            while(iterRightRect != vecEffectiveRightRect.end())
            {//与有效的矩形比较，去掉重合的部分

                std::vector<YSegment> vecYS = *iterRightYsegment;

                for(size_t k = 0; k < vecYS.size(); k++)
                {//已存在的矩形对应的有效线段
                    YSegment ysk = vecYS.at(k);

                    std::vector<YSegment>::iterator iterYSAdd = vecYSAdd.begin();

                    while(iterYSAdd != vecYSAdd.end())
                    {//新增加的相邻矩形的有效线段
                        YSegment &ysl = *iterYSAdd;

                        if(ysl.startY < ysk.startY && ysl.endY >= ysk.startY && ysl.endY < ysk.endY)
                        {
                            ysl.endY = ysk.startY;
                            iterYSAdd++;
                        }
                        else if (ysl.startY >= ysk.startY && ysl.endY <= ysk.endY)
                        {
                            //被已有的边包含，不计算
                            iterYSAdd = vecYSAdd.erase(iterYSAdd);
                        }
                        else if (ysl.startY >= ysk.startY && ysl.startY <= ysk.endY && ysl.endY > ysk.endY)
                        {
                            ysl.startY = ysk.endY;
                            iterYSAdd++;
                        }
                        else if (ysl.startY < ysk.startY && ysl.endY > ysk.endY)
                        {//分成两段
                            YSegment ysAdd;
                            ysAdd.startY = ysk.endY;
                            ysAdd.endY = ysl.endY;
                            ysl.endY = ysk.startY;

                            vecYSAdd.push_back(ysAdd);
                            iterYSAdd = vecYSAdd.begin();
                        }
                        else
                        {
                            iterYSAdd++;
                        }
                    }
                }

                iterRightRect++;
                iterRightYsegment++;
            }

            //与所有有效矩形比较的结果，有线段则加入
            if(!vecYSAdd.empty())
            {
                vecEffectiveRightRect.push_back(r);
                vecEffectiveRightYsegment.push_back(vecYSAdd);
            }

        }
    }

    double nAbove = 0, nBelow = 0, nLeft = 0, nRight = 0;
    for(size_t i = 0; i < vecEffectiveAboveXsegment.size(); i++)
    {
        bool isDeleted = false;
        for(size_t k = 0; k < vecDeletedRect.size(); k++)
        {//是否已经删除
            double dToleranceX = vecEffectiveAboveRect.at(i).x - vecDeletedRect.at(k).x;
            double dToleranceY = vecEffectiveAboveRect.at(i).y - vecDeletedRect.at(k).y;
            if(dToleranceX > -m_dTolerance && dToleranceX < m_dTolerance && dToleranceY > -m_dTolerance && dToleranceY < m_dTolerance)
            {
                isDeleted = true;
                break;
            }
        }
        if(!isDeleted)
        {
            std::vector<XSegment> vecAboveXSegment = vecEffectiveAboveXsegment.at(i);
            for(size_t j = 0;  j < vecAboveXSegment.size(); j++)
            {
                XSegment xsj = vecAboveXSegment.at(j);
                nAbove += xsj.endX - xsj.startX;
            }
        }
    }

    for(size_t i = 0; i < vecEffectiveBelowXsegment.size(); i++)
    {
        bool isDeleted = false;
        for(size_t k = 0; k < vecDeletedRect.size(); k++)
        {//是否已经删除
            double dToleranceX = vecEffectiveBelowRect.at(i).x - vecDeletedRect.at(k).x;
            double dToleranceY = vecEffectiveBelowRect.at(i).y - vecDeletedRect.at(k).y;
            if(dToleranceX > -m_dTolerance && dToleranceX < m_dTolerance && dToleranceY > -m_dTolerance && dToleranceY < m_dTolerance)
            {
                isDeleted = true;
                break;
            }
        }
        if(!isDeleted)
        {
            std::vector<XSegment> vecBelowXSegment = vecEffectiveBelowXsegment.at(i);
            for(size_t j = 0;  j < vecBelowXSegment.size(); j++)
            {
                nBelow += vecBelowXSegment.at(j).endX - vecBelowXSegment.at(j).startX;
            }
        }
    }

    for(size_t i = 0; i < vecEffectiveLeftYsegment.size(); i++)
    {
        bool isDeleted = false;
        for(size_t k = 0; k < vecDeletedRect.size(); k++)
        {//是否已经删除
            double dToleranceX = vecEffectiveLeftRect.at(i).x - vecDeletedRect.at(k).x;
            double dToleranceY = vecEffectiveLeftRect.at(i).y - vecDeletedRect.at(k).y;
            if(dToleranceX > -m_dTolerance && dToleranceX < m_dTolerance && dToleranceY > -m_dTolerance && dToleranceY < m_dTolerance)
            {
                isDeleted = true;
                break;
            }
        }
        if(!isDeleted)
        {
            std::vector<YSegment> vecLeftYSegment = vecEffectiveLeftYsegment.at(i);
            for(size_t j = 0;  j < vecLeftYSegment.size(); j++)
            {
                nLeft += vecLeftYSegment.at(j).endY - vecLeftYSegment.at(j).startY;
            }
        }
    }

    for(size_t i = 0; i < vecEffectiveRightYsegment.size(); i++)
    {
        bool isDeleted = false;
        for(size_t k = 0; k < vecDeletedRect.size(); k++)
        {//是否已经删除
            double dToleranceX = vecEffectiveRightRect.at(i).x - vecDeletedRect.at(k).x;
            double dToleranceY = vecEffectiveRightRect.at(i).y - vecDeletedRect.at(k).y;
            if(dToleranceX > -m_dTolerance && dToleranceX < m_dTolerance && dToleranceY > -m_dTolerance && dToleranceY < m_dTolerance)
            {
                isDeleted = true;
                break;
            }
        }
        if(!isDeleted)
        {
            std::vector<YSegment> vecRightYSegment = vecEffectiveRightYsegment.at(i);
            for(size_t j = 0;  j < vecRightYSegment.size(); j++)
            {
                nRight += vecRightYSegment.at(j).endY - vecRightYSegment.at(j).startY;
            }
        }
    }

    vecDeletedRect.push_back(s);

    RectF rc = s;
    if(nAbove >= nBelow && nAbove >= nLeft && nAbove >= nRight)
    {
        RVector lt(rc.x, rc.y);
        double dOffset = m_dOffset < rc.width ? m_dOffset : rc.width/4;
        lt.setX(lt.x + dOffset);
        return lt;
    }
    else if(nBelow >= nAbove && nBelow >= nLeft && nBelow >= nRight)
    {
        RVector rb(rc.x + rc.width, rc.y + rc.height);
        double dOffset = m_dOffset < rc.width ? m_dOffset : rc.width/4;
        rb.setX(rb.x - dOffset);
        return rb;
    }
    else if(nLeft >= nBelow && nLeft >= nAbove && nLeft >= nRight)
    {
        RVector lb(rc.x, rc.y + rc.height);
        double dOffset = m_dOffset < rc.height ? m_dOffset : rc.height/4;
        lb.setY(lb.y - dOffset);
        return lb;
    }
    else/* if(nRight >= nBelow && nRight >= nLeft && nRight >= nAbove)*/
    {
        RVector rt(rc.x + rc.width, rc.y);
        double dOffset = m_dOffset < rc.height ? m_dOffset : rc.height/4;
        rt.setY(rt.y + dOffset);
        return rt;
    }
}

void CuttingPointSearcher::sortByPosition(std::vector<RectF> &vec)
{
    std::vector<RectF> vecRectSort;

    std::vector<RectF>::iterator iter = vec.begin();
    for(; iter != vec.end();)
    {
        double dAboveLength = 0;

        double dBelowLength = 0;

        double dLeftLength = 0;

        double dRightLength = 0;

        QVector<XSegment> vecAboveSegment;
        QVector<XSegment> vecBelowSegment;
        QVector<YSegment> vecRightSegment;
        QVector<YSegment> vecLeftSegment;

        if(vec.size() > 1)
        {
            RectF& rc = *iter;

            for (size_t j = 0; j < vec.size(); j++)
            {
                RectF& rcj = vec.at(j);
                if(rc != rcj)
                {
                    if( isBelow(rc, rcj) )
                    {
                        belowSegment(rc, rcj, vecBelowSegment);
                    }
                }
            }

            for (size_t j = 0; j < vec.size(); j++)
            {
                RectF& rcj = vec.at(j);
                if(rc != rcj)
                {
                    if( isRight(rc, rcj) )
                    {
                        rightSegment(rc, rcj, vecRightSegment);
                    }
                }
            }

            for (size_t j = 0; j < vec.size(); j++)
            {
                RectF& rcj = vec.at(j);
                if(rc != rcj)
                {
                    if( isAbove(rc, rcj) )
                    {
                        aboveSegment(rc, rcj, vecAboveSegment);
                    }
                }
            }

            for (size_t j = 0; j < vec.size(); j++)
            {
                RectF& rcj = vec.at(j);
                if(rc != rcj)
                {
                    if( isLeft(rc, rcj) )
                    {
                        leftSegment(rc, rcj, vecLeftSegment);
                    }
                }
            }

            int nSize = vecAboveSegment.size();
            for(int i = 0; i < nSize; i++)
            {
                dAboveLength += (vecAboveSegment.at(i).endX - vecAboveSegment.at(i).startX);
            }

            nSize = vecBelowSegment.size();
            for(int i = 0; i < nSize; i++)
            {
                dBelowLength += (vecBelowSegment.at(i).endX - vecBelowSegment.at(i).startX);
            }

            nSize = vecLeftSegment.size();
            for(int i = 0; i < nSize; i++)
            {
                dLeftLength += (vecLeftSegment.at(i).endY - vecLeftSegment.at(i).startY);
            }

            nSize = vecRightSegment.size();
            for(int i = 0; i < nSize; i++)
            {
                dRightLength += (vecRightSegment.at(i).endY - vecRightSegment.at(i).startY);
            }

            if(dBelowLength == 0 && dRightLength == 0)
            {
                if(dAboveLength >= dLeftLength)
                {
                    RVector lt(rc.x, rc.y);
                    double dOffset = m_dOffset < rc.width ? m_dOffset : rc.width/4;
                    //lt.setX(lt.x + dOffset);
                    lt.setX(lt.x + rc.width - dOffset);
                    rc.ctx = lt.x;
                    rc.cty = lt.y;
                }
                else if(dAboveLength < dLeftLength)
                {
                    RVector lb(rc.x, rc.y+rc.height);
                    double dOffset = m_dOffset < rc.height ? m_dOffset : rc.height/4;
                    //lb.setY(lb.y - dOffset);
                    lb.setY(lb.y - rc.height + dOffset);
                    rc.ctx = lb.x;
                    rc.cty = lb.y;
                }

                vecRectSort.push_back(rc);

                vec.erase(iter);
                iter = vec.begin();
            }
            else if(dRightLength == 0 && dAboveLength == 0)
            {
                if(dLeftLength >= dBelowLength)
                {
                    RVector lb(rc.x, rc.y+rc.height);
                    double dOffset = m_dOffset < rc.height ? m_dOffset : rc.height/4;
                    //lb.setY(lb.y - dOffset);
                    lb.setY(lb.y  - rc.height + dOffset);
                    rc.ctx = lb.x;
                    rc.cty = lb.y;
                }
                else if(dLeftLength < dBelowLength)
                {
                    RVector rb(rc.x+rc.width, rc.y+rc.height);
                    double dOffset = m_dOffset < rc.width ? m_dOffset : rc.width/4;
                    //rb.setX(rb.x - dOffset);
                    rb.setX(rb.x - rc.width + dOffset);
                    rc.ctx = rb.x;
                    rc.cty = rb.y;
                }

                vecRectSort.push_back(rc);

                vec.erase(iter);
                iter = vec.begin();
            }
            else if(dAboveLength == 0 && dLeftLength == 0)
            {
                if(dBelowLength >= dRightLength)
                {
                    RVector rb(rc.x+rc.width, rc.y+rc.height);
                    double dOffset = m_dOffset < rc.width ? m_dOffset : rc.width/4;
                    //rb.setX(rb.x - dOffset);
                    rb.setX(rb.x  - rc.width +  dOffset);
                    rc.ctx = rb.x;
                    rc.cty = rb.y;
                }
                else if(dBelowLength < dRightLength)
                {
                    RVector rt(rc.x+rc.width, rc.y);
                    double dOffset = m_dOffset < rc.height ? m_dOffset : rc.height/4;
                    //rt.setY(rt.y +  dOffset);
                    rt.setY(rt.y + rc.height - dOffset);
                    rc.ctx = rt.x;
                    rc.cty = rt.y;
                }

                vecRectSort.push_back(rc);

                vec.erase(iter);
                iter = vec.begin();
            }
            else if(dLeftLength == 0 && dBelowLength == 0)
            {
                if(dRightLength >= dAboveLength)
                {
                    RVector rt(rc.x+rc.width, rc.y);
                    double dOffset = m_dOffset < rc.height ? m_dOffset : rc.height/4;
                    //rt.setY(rt.y + dOffset);
                    rt.setY(rt.y + rc.height - dOffset);
                    rc.ctx = rt.x;
                    rc.cty = rt.y;
                }
                else if(dRightLength < dAboveLength)
                {
                    RVector lt(rc.x, rc.y);
                    double dOffset = m_dOffset < rc.width ? m_dOffset : rc.width/4;
                    //lt.setX(lt.x + dOffset);
                    lt.setX(lt.x + rc.width - dOffset);
                    rc.ctx = lt.x;
                    rc.cty = lt.y;
                }

                vecRectSort.push_back(rc);

                vec.erase(iter);
                iter = vec.begin();
            }
            else
            {
                ++iter;
            }
        }
        else if (vec.size() == 1)
        {
            std::vector<RectF>::iterator it = vec.begin();
            RectF& rc = *it;
            if(rc.width <= rc.height)
            {
                RVector lb(rc.x, rc.y+rc.height);
                double dOffset = m_dOffset < rc.height ? m_dOffset : rc.height/4;
                //lb.setY(lb.y - dOffset);
                lb.setY(lb.y - rc.height + dOffset);
                rc.ctx = lb.x;
                rc.cty = lb.y;
            }
            else
            {
                RVector lt(rc.x, rc.y);
                double dOffset = m_dOffset < rc.width ? m_dOffset : rc.width/4;
                //lt.setX(lt.x + dOffset);
                lt.setX(lt.x + rc.width - dOffset);
                rc.ctx = lt.x;
                rc.cty = lt.y;
            }
            vecRectSort.push_back(rc);
            iter = vec.erase(iter);
        }
    }

    vec = vecRectSort;

}

void CuttingPointSearcher::sortByXY(std::vector<RectF> &vec)
{
    size_t nSize = vec.size();
    for(size_t i = 0; i < nSize; i++)
    {
        RectF& rcI = vec.at(i);

        for (size_t j = i+1; j < nSize; j++)
        {
            RectF& rcJ = vec.at(j);
            if((rcI.width-1) * (rcI.height-1) == (rcJ.width-1) * (rcJ.height-1))
            {
                if(rcJ.x == rcI.x && rcJ.y > rcI.y)
                {
                    RectF rect = rcI;
                    rcI = rcJ;
                    rcJ = rect;
                }
                else if(rcJ.y == rcI.y && rcJ.x > rcI.x)
                {
                    RectF rect = rcI;
                    rcI = rcJ;
                    rcJ = rect;
                }
                //vec.replace(i, rcJ);
                //vec.replace(j, rect);
            }
        }
    }
    return;
}

void CuttingPointSearcher::sortByArea(std::vector<RectF> &vec)
{
    size_t nSize = vec.size();
    for(size_t i = 0; i < nSize; i++)
    {
        RectF& rcI = vec.at(i);

        for (size_t j = i+1; j < nSize; j++)
        {
            RectF& rcJ = vec.at(j);
            if((rcI.width-1) * (rcI.height-1) > (rcJ.width-1) * (rcJ.height-1))
            {
                RectF rect = rcI;
                rcI = rcJ;
                rcJ = rect;
            }
        }
    }
    return;
}

bool CuttingPointSearcher::isAbove(const RectF &s, const RectF &r)
{
    RVector sLt(s.x, s.y);
    RVector sRt(s.x+s.width, s.y);
    RVector rLb(r.x, r.y+r.height);
    RVector rRb(r.x+r.width, r.y+r.height);

    if(sLt.y >= rLb.y)
    {
        if(sLt.x >= rRb.x)
        {
            return false;
        }
        else if(sRt.x <= rLb.x)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool CuttingPointSearcher::isBelow(const RectF &s, const RectF &r)
{
    RVector sLb(s.x, s.y+s.height);
    RVector sRb(s.x+s.width, s.y+s.height);
    RVector rRt(r.x+r.width, r.y);
    RVector rLt(r.x, r.y);

    if(sLb.y <= rLt.y)
    {
        if(sLb.x >= rRt.x)
        {
            return false;
        }
        else if(sRb.x <= rLt.x)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool CuttingPointSearcher::isLeft(const RectF &s, const RectF &r)
{
    RVector sLb(s.x, s.y+s.height);
    RVector sLt(s.x, s.y);
    RVector rRb(r.x+r.width, r.y+r.height);
    RVector rRt(r.x+r.width, r.y);

    if(sLb.x >= rRb.x)
    {
        if(sLt.y >= rRb.y)
        {
            return false;
        }
        else if(sLb.y <= rRt.y)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool CuttingPointSearcher::isRight(const RectF &s, const RectF &r)
{
    RVector sRb(s.x+s.width, s.y+s.height);
    RVector sRt(s.x+s.width, s.y);
    RVector rLb(r.x, r.y+r.height);
    RVector rLt(r.x, r.y);

    if(sRb.x <= rLb.x)
    {
        if(sRt.y >= rLb.y)
        {
            return false;
        }
        else if(sRb.y <= rLt.y)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
    }
}

void CuttingPointSearcher::aboveSegment(const RectF &s, const RectF &r, QVector<XSegment> &vecXSegment)
{
    double dDistance = s.y - (r.y + r.height);
    if(dDistance > m_dDistance)
    {
        return;
    }

    XSegment xs;
    if(r.x <= s.x && (r.x + r.width) >= (s.x + s.width))
    {
        xs.startX = s.x;
        xs.endX = s.x + s.width;
    }
    else if(r.x <= s.x && (r.x + r.width) >= s.x && (r.x + r.width) <= (s.x + s.width))
    {
        xs.startX = s.x;
        xs.endX = r.x + r.width;
    }
    else if(r.x >= s.x && (r.x + r.width) <= (s.x + s.width))
    {
        xs.startX = r.x;
        xs.endX = r.x + r.width;
    }
    else if(r.x >= s.x && r.x <= (s.x + s.width) && (r.x + r.width) >= (s.x + s.width))
    {
        xs.startX = r.x;
        xs.endX = s.x + s.width;
    }

    int nSize = vecXSegment.size();

    if(nSize == 0)
    {
        vecXSegment.append(xs);
    }
    else
    {
        for(int i = 0; i < nSize; i++)
        {
            XSegment xsi = vecXSegment.at(i);
            if(xs.startX <= xsi.startX && xs.endX >= xsi.endX)
            {
                vecXSegment.replace(i, xs);
            }
            else if(xs.startX <= xsi.startX && xs.endX >= xsi.startX && xs.endX <= xsi.endX)
            {
                xsi.startX = xs.startX;
                vecXSegment.replace(i, xsi);
            }
            else if (xs.startX >= xsi.startX && xs.endX <= xsi.endX)
            {

            }
            else if(xs.startX >= xsi.startX && xs.startX <= xsi.endX && xs.endX >= xsi.endX)
            {
                xsi.endX = xs.endX;
                vecXSegment.replace(i, xsi);
            }
            else
            {
                vecXSegment.append(xs);
            }
        }
    }
}

void CuttingPointSearcher::belowSegment(const RectF &s, const RectF &r, QVector<XSegment> &vecXSegment)
{
    double dDistance = r.y - (s.y + s.height);
    if(dDistance > m_dDistance)
    {
        return;
    }

    XSegment xs;
    if(r.x <= s.x && (r.x + r.width) >= (s.x + s.width))
    {
        xs.startX = s.x;
        xs.endX = s.x + s.width;
    }
    else if(r.x <= s.x && (r.x + r.width) >= s.x && (r.x + r.width) <= (s.x + s.width))
    {
        xs.startX = s.x;
        xs.endX = r.x + r.width;
    }
    else if(r.x >= s.x && (r.x + r.width) <= (s.x + s.width))
    {
        xs.startX = r.x;
        xs.endX = r.x + r.width;
    }
    else if(r.x >= s.x && r.x <= (s.x + s.width) && (r.x + r.width) >= (s.x + s.width))
    {
        xs.startX = r.x;
        xs.endX = s.x + s.width;
    }

    int nSize = vecXSegment.size();
    if(nSize == 0)
    {
        vecXSegment.append(xs);
    }
    else
    {
        for(int i = 0; i < nSize; i++)
        {
            XSegment xsi = vecXSegment.at(i);
            if(xs.startX <= xsi.startX && xs.endX >= xsi.endX)
            {
                vecXSegment.replace(i, xsi);
            }
            else if(xs.startX <= xsi.startX && xs.endX >= xsi.startX && xs.endX <= xsi.endX)
            {
                xsi.startX = xs.startX;
                vecXSegment.replace(i, xsi);
            }
            else if (xs.startX >= xsi.startX && xs.endX <= xsi.endX)
            {

            }
            else if(xs.startX >= xsi.startX && xs.startX <= xsi.endX && xs.endX >= xsi.endX)
            {
                xsi.endX = xs.endX;
                vecXSegment.replace(i, xsi);
            }
            else
            {
                vecXSegment.append(xs);
            }
        }
    }
}

void CuttingPointSearcher::rightSegment(const RectF &s, const RectF &r, QVector<YSegment> &vecYSegment)
{
    double dDistance = r.x - (s.x + s.width);
    if(dDistance > m_dDistance)
    {
        return;
    }

    YSegment ys;

    if(r.y <= s.y && (r.y + r.height) >= (s.y + s.height))
    {
        ys.startY = s.y;
        ys.endY = s.y + s.height;
    }
    else if(r.y <= s.y && (r.y + r.height) >= s.y && (r.y + r.height) <= (s.y + s.height))
    {
        ys.startY = s.y;
        ys.endY = r.y + r.height;
    }
    else if(r.y >= s.y && (r.y + r.height) <= (s.y + s.height))
    {
        ys.startY = r.y;
        ys.endY = r.y + r.height;
    }
    else if (r.y >= s.y && r.y <= (s.y + s.height) && (r.y + r.height) >= (s.y + s.height))
    {
        ys.startY = r.y;
        ys.endY = s.y + s.height;
    }

    int nSize = vecYSegment.size();
    if(nSize == 0)
    {
        vecYSegment.append(ys);
    }
    else
    {

        for(int i = 0; i < nSize; i++)
        {
            YSegment ysi = vecYSegment.at(i);

            if(ys.startY <= ysi.startY && ys.endY >= ysi.endY)
            {
                vecYSegment.replace(i, ys);
            }
            else if(ys.startY >= ysi.startY && ys.startY <= ysi.endY && ys.endY >= ysi.endY)
            {
                ysi.endY = ys.endY;
                vecYSegment.replace(i, ysi);
            }
            else if(ys.startY >= ysi.startY && ys.endY <= ysi.endY)
            {

            }
            else if(ys.startY <= ysi.startY && ys.endY >= ysi.startY && ys.endY <= ysi.endY)
            {
                ysi.startY = ys.startY;
                vecYSegment.replace(i, ysi);
            }
            else
            {
                vecYSegment.append(ys);
            }
        }
    }
}
void CuttingPointSearcher::leftSegment(const RectF &s, const RectF &r, QVector<YSegment> &vecYSegment)
{
    double dDistance = s.x - (r.x + r.width);
    if(dDistance > m_dDistance)
    {
        return;
    }

    YSegment ys;

    if(r.y <= s.y && (r.y + r.height) >= (s.y + s.height))
    {
        ys.startY = s.y;
        ys.endY = s.y + s.height;
    }
    else if(r.y <= s.y && (r.y + r.height) >= s.y && (r.y + r.height) <= (s.y + s.height))
    {
        ys.startY = s.y;
        ys.endY = r.y + r.height;
    }
    else if(r.y >= s.y && (r.y + r.height) <= (s.y + s.height))
    {
        ys.startY = r.y;
        ys.endY = r.y + r.height;
    }
    else if (r.y >= s.y && r.y <= (s.y + s.height) && (r.y + r.height) >= (s.y + s.height))
    {
        ys.startY = r.y;
        ys.endY = s.y + s.height;
    }

    int nSize = vecYSegment.size();
    if(nSize == 0)
    {
        vecYSegment.append(ys);
    }
    else
    {
        for(int i = 0; i < nSize; i++)
        {
            YSegment ysi = vecYSegment.at(i);

            if(ys.startY <= ysi.startY && ys.endY >= ysi.endY)
            {
                vecYSegment.replace(i, ys);
            }
            else if(ys.startY >= ysi.startY && ys.startY <= ysi.endY && ys.endY >= ysi.endY)
            {
                ysi.endY = ys.endY;
                vecYSegment.replace(i, ysi);
            }
            else if(ys.startY >= ysi.startY && ys.endY <= ysi.endY)
            {

            }
            else if(ys.startY <= ysi.startY && ys.endY >= ysi.startY && ys.endY <= ysi.endY)
            {
                ysi.startY = ys.startY;
                vecYSegment.replace(i, ysi);
            }
            else
            {
                vecYSegment.append(ys);
            }
        }
    }
}
