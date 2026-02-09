#pragma once

#include "qb2joint.h"

class QB2FilterJoint : public QB2Joint
{
    Q_OBJECT

public:
    explicit QB2FilterJoint(QObject *parent = nullptr);
    ~QB2FilterJoint();

protected:
    void createJoint() override;
};
