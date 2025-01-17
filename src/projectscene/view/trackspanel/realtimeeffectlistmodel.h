/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "modularity/ioc.h"
#include "context/iglobalcontext.h"
#include "realtimeeffectmenumodelbase.h"
#include "effects/effects_base/irealtimeeffectservice.h"
#include "effects/effects_base/ieffectsprovider.h"
#include <QObject>
#include <map>

namespace au::projectscene {
class ModelEffectItem : public QObject, public muse::Injectable
{
    Q_OBJECT

    muse::Inject<effects::IEffectsProvider> effectsProvider;

public:
    ModelEffectItem(QObject* parent, effects::EffectStateId effectState);

    const effects::EffectStateId effectStateId;
    Q_INVOKABLE QString effectName() const;
    Q_INVOKABLE void showDialog();
};

class RealtimeEffectListModel : public RealtimeEffectMenuModelBase
{
    Q_OBJECT

    Q_PROPERTY(QVariantList availableEffects READ availableEffects NOTIFY availableEffectsChanged)

    muse::Inject<context::IGlobalContext> globalContext;
    muse::Inject<effects::IRealtimeEffectService> realtimeEffectService;

public:
    explicit RealtimeEffectListModel(QObject* parent = nullptr);

    Q_INVOKABLE void handleMenuItemWithState(const QString& menuItemId, const ModelEffectItem*);
    QVariantList availableEffects();

signals:
    void availableEffectsChanged();

private:
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex& index, int role) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

private:
    enum RoleNames
    {
        rItemData = Qt::UserRole + 1
    };

    void doLoad() override;
    void populateMenu() override;

    void setListenerOnCurrentTrackeditProject();
    void insertEffect(effects::TrackId trackId, effects::EffectChainLinkIndex index, const effects::EffectStateId& item);
    void removeEffect(effects::TrackId trackId, effects::EffectChainLinkIndex index, const effects::EffectStateId& item);

    using EffectList = std::vector<ModelEffectItem*>;
    std::map<effects::TrackId, EffectList> m_trackEffectLists;
};
}
