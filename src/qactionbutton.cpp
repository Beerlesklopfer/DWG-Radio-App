#include "qactionbutton.h"

QActionButton::QActionButton(QString text, QWidget *parent) :
    QPushButton(parent)
{
    QPushButton::setText(text);
}

void QActionButton::setAction(QAction *action)
{
    // if I've got already an action associated to the button
    // remove all connections
    if (actionOwner && actionOwner != action) {
        disconnect(actionOwner, &QAction::changed,
                   this, &QActionButton::updateButtonStatusFromAction);
        disconnect(this, &QActionButton::clicked,
                   actionOwner, &QAction::trigger);
    }

    // store the action
    actionOwner = action;

    // configure the button
    updateButtonStatusFromAction();

    // connect the action and the button
    // so that when the action is changed the
    // button is changed too!
    connect(action, &QAction::changed,
            this, &QActionButton::updateButtonStatusFromAction);

    // connect the button to the slot that forwards the
    // signal to the action
    connect(this, &QActionButton::clicked,
            actionOwner, &QAction::trigger);
}

void QActionButton::updateButtonStatusFromAction()
{
    if (!actionOwner)
        return;
    setText(actionOwner->text());
    setStatusTip(actionOwner->statusTip());
    setToolTip(actionOwner->toolTip());
    setIcon(actionOwner->icon());
    setEnabled(actionOwner->isEnabled());
    setCheckable(actionOwner->isCheckable());
    setChecked(actionOwner->isChecked());
}
