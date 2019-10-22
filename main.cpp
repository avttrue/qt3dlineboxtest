#include <QGuiApplication>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

Qt3DCore::QEntity *createEntityBox(const QVector3D &min,
                                   const QVector3D &max,
                                   const QColor &color,
                                   Qt3DCore::QEntity *parent)
{
    auto geometry = new Qt3DRender::QGeometry;

    QByteArray bufferBytes;
    bufferBytes.resize(3 * 8 * sizeof(float));
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = min.x(); *positions++ = min.y(); *positions++ = min.z();
    *positions++ = max.x(); *positions++ = min.y(); *positions++ = min.z();
    *positions++ = max.x(); *positions++ = min.y(); *positions++ = max.z();
    *positions++ = min.x(); *positions++ = min.y(); *positions++ = max.z();
    *positions++ = min.x(); *positions++ = max.y(); *positions++ = min.z();
    *positions++ = max.x(); *positions++ = max.y(); *positions++ = min.z();
    *positions++ = max.x(); *positions++ = max.y(); *positions++ = max.z();
    *positions++ = min.x(); *positions++ = max.y(); *positions++ = max.z();

    auto *vertexBuffer = new Qt3DRender::QBuffer(geometry);
    vertexBuffer->setData(bufferBytes);

    auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexBuffer);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(8);
    geometry->addAttribute(positionAttribute);

    QByteArray indexBytes;
    indexBytes.resize(24 * sizeof(unsigned int));
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());

    *indices++ = 0; *indices++ = 1;
    *indices++ = 1; *indices++ = 2;
    *indices++ = 2; *indices++ = 3;
    *indices++ = 3; *indices++ = 0;
    *indices++ = 0; *indices++ = 4;
    *indices++ = 1; *indices++ = 5;
    *indices++ = 2; *indices++ = 6;
    *indices++ = 3; *indices++ = 7;
    *indices++ = 4; *indices++ = 5;
    *indices++ = 5; *indices++ = 6;
    *indices++ = 6; *indices++ = 7;
    *indices++ = 7; *indices++ = 4;

    auto indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(24);
    geometry->addAttribute(indexAttribute);

    geometry->setParent(parent);

    auto lineEntity = new Qt3DCore::QEntity(parent);

    auto line = new Qt3DRender::QGeometryRenderer(lineEntity);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    auto material = new Qt3DExtras::QDiffuseSpecularMaterial(lineEntity);
    material->setAmbient(color);

    lineEntity->addComponent(line);
    lineEntity->addComponent(material);

    return lineEntity;
}

int main(int argc, char *argv[])
{
    QGuiApplication application(argc, argv);
    Qt3DExtras::Qt3DWindow window;

    auto scene = new Qt3DCore::QEntity;
    window.setRootEntity(scene);

    auto sphere = new Qt3DCore::QEntity(scene);

    auto transform = new Qt3DCore::QTransform;
    transform->setTranslation(QVector3D(0.0f, 0.0f, -10.0f));

    auto material = new Qt3DExtras::QPhongMaterial;
    material->setAmbient(Qt::blue);

    auto spheremesh = new Qt3DExtras::QSphereMesh;
    spheremesh->setRadius(15.0);
    spheremesh->setSlices(32);
    spheremesh->setRings(32);

    sphere->addComponent(transform);
    sphere->addComponent(material);
    sphere->addComponent(spheremesh);

    createEntityBox(QVector3D(-15.0f, -15.0f, -15.0f), QVector3D(15.0f, 15.0f, 15.0f), Qt::red, sphere);

    auto camera = window.camera();
    camera->lens()->setPerspectiveProjection(60.0f, static_cast<float>(window.width()) / window.height(), 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0.0f, 0.0f, 100.0f));
    camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

    auto camController = new Qt3DExtras::QFirstPersonCameraController(scene);
    camController->setCamera(camera);

    window.show();
    return application.exec();
}
