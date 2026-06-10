import QtQuick
import Box2D 1.0

Body {
    type: Body.Static

    fixtures: [
        Fixture {
            sensor: true
            shape: CircleShape { radius: 6; fillColor: '#bfb529' }
        }
    ]
}
