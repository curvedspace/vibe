
#ifndef QUBE_BREADCRUMBBAR_P_H
#define QUBE_BREADCRUMBBAR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the on API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace Qube
{
    namespace Gui
    {
        class BreadCrumbBar;
        class BreadCrumbComboBox;
        class AbstractBreadCrumbModel;

        class BreadCrumbBarPrivate
        {
            Q_DECLARE_PUBLIC(BreadCrumbBar);
        public:
            BreadCrumbBarPrivate(BreadCrumbBar *q_ptr);
            ~BreadCrumbBarPrivate();

            bool flat;
            bool editable;
            BreadCrumbComboBox *comboBox;
            AbstractBreadCrumbModel *model;

private:
BreadCrumbBar *q_ptr;
        };
    }
}

#endif // QUBE_BREADCRUMBBAR_P_H
